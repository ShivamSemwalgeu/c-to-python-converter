interface Props {
  cCode: string;
  onConvert: (lines: string[]) => void;
}

export default function ConvertButton({ cCode, onConvert }: Props) {
  const handleClick = async () => {
    const response = await fetch("http://localhost:5000/translate", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ code: cCode }),
    });
    const data = await response.json();
    const lines = data.stdout.split("\n").filter(Boolean);
    onConvert(lines);
  };

  return <button onClick={handleClick}>Convert to Python</button>;
}
