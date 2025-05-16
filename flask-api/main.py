from flask import Flask, request, jsonify
from flask_cors import CORS
import os
import subprocess

app = Flask(__name__)
CORS(app)

UPLOAD_FOLDER = "uploads"
CONVERTED_FOLDER = "converted"

os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(CONVERTED_FOLDER, exist_ok=True)


@app.route("/")
def home():
    return "Hello World"


@app.route("/translate", methods=["POST"])
def translate():
    data = request.get_json()
    if not data:
        return jsonify({"error": "No data provided"}), 400

    code = data.get("code")
    if not code:
        return jsonify({"error": "No code provided"}), 400

    # Write code to a file in the uploads folder
    source_file_path = os.path.join(UPLOAD_FOLDER, "program.c")

    with open(source_file_path, "w") as source_file:
        source_file.write(code)

    output_file_path = os.path.join(CONVERTED_FOLDER, "program")

    try:
        run_proc = subprocess.run(
            ["./converter", source_file_path],
            capture_output=True,
            text=True,
            timeout=5
        )

    except subprocess.TimeoutExpired:
        return jsonify({"error": "Execution timed out"}), 408

    finally:
        # Clean up files
        if os.path.exists(source_file_path):
            os.remove(source_file_path)
        if os.path.exists(output_file_path):
            os.remove(output_file_path)

    return jsonify({
        "message": "Code received successfully",
        "stdout": run_proc.stdout,
        "stderr": run_proc.stderr,
        "returncode": run_proc.returncode
    }), 200


if __name__ == "__main__":
    app.run(debug=True)
