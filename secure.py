# Refrences:
# 1. https://realpython.com/blog/python/introduction-to-flask-part-2-creating-a-login-page/
# 2. http://pythonprogramming.net/flask-registration-tutorial/?completed=/flask-user-registration-form-tutorial/
# 3. https://www.youtube.com/watch?v=AE2nnSlC3TU
# 4. http://flask.pocoo.org/docs/0.10/patterns/fileuploads/#uploading-files

from flask import Flask, render_template, flash, redirect, url_for, request, session, send_from_directory
# from flask_sslify import SSLify
import os
import subprocess
import sys
import uuid
from werkzeug import secure_filename
import MySQLdb
from MySQLdb import escape_string as thwart
from wtforms import Form, TextField, PasswordField, validators
from passlib.hash import sha256_crypt
from functools import wraps

# from OpenSSL import SSL
# context = SSL.Context(SSL.SSLv23_METHOD)
# context.use_privatekey_file('server.key')
# context.use_certificate_file('server.crt')

UPLOAD_FOLDER = 'C:\Users\KAUSTUBH1\PycharmProjects\secure'
ALLOWED_EXTENSIONS = set(['txt', 'CPP', 'java', 'py', 'php', 'c'])

app = Flask(__name__)
# sslify = SSLify(app)
app.secret_key = "kaustubh"
# app.debug = True
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS

def connection():
    conn = MySQLdb.connect(host="localhost",port = 3306,db = "test_db", user = 'root', passwd = 'password')
    c = conn.cursor()
    return c, conn

def login_required(f):
    @wraps(f)
    def wrap(*args, **kwargs):
        if 'logged_in' in session:
            return f(*args, **kwargs)
        else:
            flash("You need to login first")
            return redirect(url_for('login'))
    return wrap

@app.route('/', methods=['GET', 'POST'])
def login():
    try:
        c, conn = connection()
        error = None
        if request.method == 'POST':
            if 'login' in request.form :
                username = request.form['username']
                password = request.form['password']
                c.execute("SELECT * from users where username = %s", [thwart(username)])
                data = c.fetchone()[4]
                # salt = uuid.uuid4().hex
                # saltpassword = str(salt + request.form['password'])
                if not sha256_crypt.verify(password , data):
                    error = 'Invalid Credentials. Please try again.'
                else :
                    flash("You are now logged in")
                    session['logged_in'] = True
                    session['username'] = username
                    return redirect(url_for('upload_file'))
                c.close()
                conn.close()
            elif 'register' in request.form :
                return redirect(url_for('register'))
        return render_template('login.html', error=error)
    except Exception as e:
        return(str(e))


class RegistrationForm(Form):
    firstname = TextField('Firstname', [validators.Length(min=4, max=20)])
    lastname = TextField('Lastname', [validators.Length(min=4, max=20)])
    username = TextField('Username', [validators.Length(min=4, max=20)])
    password = PasswordField('New Password', [ validators.Required(), validators.EqualTo('confirm', message='Passwords must match')])
    confirm = PasswordField('Repeat Password')

@app.route('/register/', methods=['GET', 'POST'])
def register():
    try:
        form = RegistrationForm(request.form)

        if request.method == "POST" and form.validate():
            firstname  = form.firstname.data
            lastname  = form.lastname.data
            username  = form.username.data
            # salt = uuid.uuid4().hex
            # encrypt_pass = str(salt + form.password.data)
            # password = sha256_crypt.encrypt(encrypt_pass)
            password = sha256_crypt.encrypt(form.password.data)
            c, conn = connection()
            c.execute("SELECT * FROM users WHERE username = (%s)", [thwart(username)])
            if c.rowcount > 0:
                flash("That username is already taken, please choose another")
                return render_template('register.html', form=form)

            else:
                c.execute("INSERT INTO users (firstname , lastname, username, password) VALUES (%s, %s, %s, %s)",
                          (thwart(firstname), thwart(lastname), thwart(username), thwart(password)))
                conn.commit()
                flash("Thanks for registering!")
                c.close()
                conn.close()
                session['logged_in'] = True
                session['username'] = username
                return redirect(url_for('login'))
        return render_template("register.html", form=form)
    except Exception as e:
        return(str(e))

@app.route('/upload', methods=['GET', 'POST'])
@login_required
def upload_file():
    try:
        if request.method == 'POST':
            if 'upload' in request.form:
                file = request.files['file']
                if file and allowed_file(file.filename):
                    filename = secure_filename(file.filename)
                    file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
                    return redirect(url_for('cppcheck', filename=filename))
            elif 'logout' in request.form:
                return redirect(url_for('logout'))
        return render_template('upload.html')
    except Exception as e:
        return(str(e))

@app.route('/uploads/<filename>', methods=['GET', 'POST'])
@login_required
def cppcheck(filename):
    try:
        sys.stdout = open("output.txt","w",0)
    except Exception as e:
        return(str(e))
    if 'logout' in request.form:
        return redirect(url_for('logout'))
    dir = os.path.dirname(__file__)
    file = os.path.join(dir,filename)
    output = os.path.join(dir,"output.txt")
    var =  "cppcheck "+ file + " 2> " + output
    subprocess.call(var , stdout=sys.stdout, stderr=sys.stdout)
    results = open("output.txt","r").read()
    flash(results)
    return render_template('results.html')

@app.route('/logout')
@login_required
def logout():
    try:
        # session.pop('logged_in', None)
        session.clear()
        flash("You have been logged out!")
        return redirect(url_for('login'))
    except Exception as e:
        return(str(e))

if __name__ == '__main__':
    app.run()

# if __name__ == '__main__':
#      app.run(host='127.0.0.1', debug=True, ssl_context=context)