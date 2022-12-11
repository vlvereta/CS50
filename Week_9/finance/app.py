import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Stock operations
BUY = "buy"
SELL = "sell"

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    users = db.execute("SELECT username, cash FROM users WHERE id = ?", user_id)
    user = users[0]

    grand_total = user["cash"]
    sharesToRenderDict = {}
    shares = db.execute("SELECT symbol, amount, type FROM operations WHERE user_id = ?", user_id)
    for share in shares:
        current = lookup(share["symbol"])
        symbol = current["symbol"]
        amount = share["amount"] if share["type"] == BUY else -share["amount"]

        if not symbol in sharesToRenderDict:
            sharesToRenderDict[symbol] = {}
            sharesToRenderDict[symbol]["total"] = 0
            sharesToRenderDict[symbol]["amount"] = amount
            sharesToRenderDict[symbol]["name"] = current["name"]
            sharesToRenderDict[symbol]["price"] = current["price"]
        else:
            sharesToRenderDict[symbol]["amount"] += amount

        total = sharesToRenderDict[symbol]["price"] * amount
        sharesToRenderDict[symbol]["total"] += total
        grand_total += total

        if sharesToRenderDict[symbol]["amount"] == 0:
            del sharesToRenderDict[symbol]

    return render_template("index.html", shares=sharesToRenderDict.values(), username=user["username"], cash=user["cash"], total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        symbol = lookup(request.form.get("symbol"))
        if symbol == None:
            return apology("no such symbol", 400)

        shares = request.form.get("shares")
        try:
            if not request.form.get("shares") or int(shares) <= 0:
                return apology("check the shares amount", 400)
        except:
            return apology("something wrong with input", 400)

        purchase = symbol["price"] * int(shares)

        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        user = rows[0]
        user_cash = user["cash"]

        if user_cash < purchase:
            return apology("not enough cash", 400)
        else:
            db.execute("INSERT INTO operations (user_id, symbol, price, amount, type) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], symbol["symbol"], symbol["price"], int(shares), BUY)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash - purchase, user["id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_id = session["user_id"]
    shares = db.execute("SELECT symbol, amount, type, timestamp FROM operations WHERE user_id = ?", user_id)

    sharesToRenderList = []
    for share in shares:
        current = lookup(share["symbol"])
        share["name"] = current["name"]
        share["price"] = current["price"]
        sharesToRenderList.append(share)

    return render_template("history.html", shares=sharesToRenderList)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        result = lookup(request.form.get("symbol"))
        if not result:
            return apology("no such a symbol", 400)

        return render_template("quoted.html", result=result)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)

        # Ensure password and confirmation match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation must match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username doesn't exist
        if len(rows) != 0:
            return apology("user with such a name already exists", 400)

        # Store the new user
        user_id = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)",
                             request.form.get("username"), generate_password_hash(request.form.get("password")))

        # Remember which user has registered
        session["user_id"] = user_id

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        symbol = request.form.get("symbol")
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        shares = request.form.get("shares")
        if not request.form.get("shares") or int(shares) <= 0:
            return apology("check the shares amount", 400)

        ownCurrent = None
        ownShares = db.execute("SELECT * FROM operations WHERE user_id = ? AND \
            symbol = ? AND type = ?", session["user_id"], symbol, BUY)
        for own in ownShares:
            if own["symbol"] == symbol:
                ownCurrent = own
                break

        if not ownCurrent or ownCurrent["amount"] < int(shares):
            return apology("something wrong with your shares", 400)

        share = lookup(symbol)
        sell = share["price"] * int(shares)

        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        user = rows[0]
        user_cash = user["cash"]

        db.execute("INSERT INTO operations (user_id, symbol, price, amount, type) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], symbol, share["price"], int(shares), SELL)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash + sell, user["id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by a link or via redirect)
    else:
        shares = db.execute("SELECT DISTINCT(symbol) FROM operations WHERE user_id = ? AND type = ?", session["user_id"], BUY)
        for share in shares:
            current = lookup(share["symbol"])
            share["name"] = current["name"]
        return render_template("sell.html", shares=shares)


@app.route("/debit", methods=["GET", "POST"])
@login_required
def debit():
    """Debit money for user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure the input is correct
        money = request.form.get("money")
        if not money or int(money) <= 0:
            return apology("provide proper amount", 400)

        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        user = rows[0]

        db.execute("UPDATE users SET cash = ? WHERE id = ?", user["cash"] + int(money), user["id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("debit.html")
