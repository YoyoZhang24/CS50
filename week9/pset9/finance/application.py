# export API_KEY=pk_f5573f7b8d0c4f7fbaf2da30e07691f1
# Personal Touch: Require users’ passwords to have at least 8 characters.


import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


# TODO (1/6)
@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    transactions = db.execute(
        "SELECT symbol, SUM(shares) as shares, type FROM transactions WHERE user_id = ? GROUP BY symbol HAVING (SUM(shares)) > 0;",
        session["user_id"],
    )
    total_cash_transactions = 0

    for transaction in transactions:
        quote = lookup(transaction["symbol"])
        transaction["name"] = quote["name"]
        transaction["price"] = quote["price"]
        transaction["total"] = transaction["price"] * transaction["shares"]
        total_cash_transactions += transaction["total"]

    total_cash = total_cash_transactions + cash[0]["cash"]
    return render_template("index.html", transactions=transactions, cash=cash[0], total_cash=total_cash)


# TODO (2/6)
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Check stock price & user cash
        symbol = request.form.get("symbol")
        price = lookup(symbol)
        shares = request.form.get("shares")

        # Check symbol validity
        if price is None:
            return apology("invalid symbol")
        price = price["price"]

        # Check shares validity
        try:
            shares = int(shares)
            if shares < 1:
                return apology("share must be a positive integer")
        except ValueError:
            return apology("share must be a positive integer")

        # Checks cash amount
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        if (price * shares) > cash:
            return apology("not enough cash")

        # Update cash count
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", price * shares, session["user_id"])

        # Record transaction
        now = datetime.now()
        date = now.strftime("%d/%m/%Y %H:%M:%S")
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, type, date) VALUES (?, ?, ?, ?, ?, ?)",
            session["user_id"],
            symbol.upper(),
            shares,
            price,
            "buy",
            date
        )
        return redirect("/")

    else:
        return render_template("buy.html")


# TODO (3/6)
@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


# TODO (4/6)
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        value = lookup(request.form.get("symbol"))
        if value == None:
            return apology("invalid symbol")
        return render_template("quoted.html", value=value)

    else:
        return render_template("quote.html")


# TODO (5/6)
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check for empty fields
        if (not username) or (not password) or (not confirmation):
            return apology("must fill in all fields")

        # Check for password length
        if (len(password) < 8):
            return apology("password too short")

        # Check for non-matching passwords
        if (password != confirmation):
            return apology("passwords must match")

        # Check for repeated username
        repeats = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(repeats) != 0:
            return apology("username taken")

        # Hashes password
        hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
        return redirect("/")

    else:
        return render_template("register.html")


# TODO (6/6)
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Check for invalid input
        if shares < 1 or not symbol:
            return apology("invalid transaction")

        # Check if user has enough shares
        transactions = db.execute(
            "SELECT SUM(shares) as shares FROM transactions WHERE user_id = ? AND symbol = ?;",
            session["user_id"],
            symbol,)[0]
        if shares > transactions["shares"]:
            return apology("not enough shares owned")

        # Update cash count
        price = lookup(symbol)["price"]
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", price * shares, session["user_id"])

        # Record transaction
        now = datetime.now()
        date = now.strftime("%d/%m/%Y %H:%M:%S")
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, type, date) values (?, ?, ?, ?, ?, ?)",
            session["user_id"],
            symbol.upper(),
            -shares,
            price,
            "sell",
            date
        )
        return redirect("/")

    else:
        transactions = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol",
            session["user_id"])
        return render_template("sell.html", transactions=transactions)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
