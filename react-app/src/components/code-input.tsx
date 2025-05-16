interface Props {
  value: string;
  onChange: (code: string) => void;
}

export default function CodeInput({ value, onChange }: Props) {
  return (
    <div>
      <h2>Paste C Code</h2>
      <textarea
        value={value}
        onChange={(e) => onChange(e.target.value)}
        rows={10}
        cols={60}
        placeholder="Paste your C code here"
      />
    </div>
  );
}
