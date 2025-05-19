interface Props {
  lines: string[];
}

export default function PythonOutput({ lines }: Props) {
  return (
    <div>
      <h2>Converted Python Code</h2>
      <pre>
        {lines.map((line, idx) => (
          <div key={idx}>{line}</div>
        ))}
      </pre>
    </div>
  );
}
