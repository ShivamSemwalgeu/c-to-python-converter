type Props = {
  label: string; // Button label (e.g., "Download Python Code")
  filename: string; // Download filename (e.g., "converted.py")
  content: string; // Actual code content
  disabled?: boolean; // Optional disabled state
};

export default function DownloadButton({
  label,
  filename,
  content,
  disabled,
}: Props) {
  const handleDownload = () => {
    const blob = new Blob([content], { type: "text/plain" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = filename;
    a.click();
    URL.revokeObjectURL(url);
  };

  return (
    <button onClick={handleDownload} disabled={disabled}>
      {label}
    </button>
  );
}
