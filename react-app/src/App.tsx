import { useState } from "react";
import "./App.css";
import CodeInput from "./components/code-input";
import ConvertButton from "./components/convert-button";
import PythonOutput from "./components/python-output";
import DownloadButton from "./components/download-button";

function App() {
  const [cCode, setCCode] = useState("");
  const [pythonLines, setPythonLines] = useState<string[]>([]);

  return (
    <div className="app">
      <h1>C to Python Converter</h1>
      <div className="code-container">
        <div className="code-box">
          <CodeInput value={cCode} onChange={setCCode} />
          <DownloadButton
            label="Download C Code"
            filename={`c_code.c`}
            content={cCode}
            disabled={!cCode.trim()}
          />
        </div>
        <div className="code-box">
          <PythonOutput lines={pythonLines} />
          <DownloadButton
            label="Download Python Code"
            filename={`python_code.py`}
            content={pythonLines.join("\n")}
            disabled={pythonLines.length === 0}
          />
        </div>
      </div>
      <ConvertButton cCode={cCode} onConvert={setPythonLines} />
    </div>
  );
}

export default App;
