# CodeSwitch: C to Python Translator

CodeSwitch is a web-based tool that translates simple C code into Python using a custom-built C parser and a Flask API backend. The React frontend provides an intuitive interface for users to paste their C code and view the translated Python output instantly.

## 🛠️ Project Structure

* **`converter.c`** — C program to parse and translate C code to Python.
* **`flask-api/main.py`** — Flask API that compiles and runs `converter`, handling incoming C code from the frontend.
* **`react-app/`** — React app for user input and output display.

---

## 📦 Setup Instructions

### 1. Build the C Translator

Compile the `converter.c` file:

```bash
gcc converter.c -o converter
```

> This creates an executable file `converter` used by the Flask backend.

---

### 2. Start the Flask API

Make sure Python and `Flask` are installed. Then, run:

```bash
python main.py
```

By default, the API runs at `http://localhost:5000`.

---

### 3. Start the React Frontend

Navigate to the frontend directory and install dependencies (if not already):

```bash
npm install
```

Then start the development server:

```bash
npm run dev
```

React app will be available at `http://localhost:5173` (or as shown in terminal).

---