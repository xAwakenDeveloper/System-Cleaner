# 🧹 System Cleaner (C++)

A simple and lightweight **Windows console application** written in **C++**, designed to automatically clean common system junk folders and temporary files to free up disk space and keep your system tidy.

---

## ✨ Features

- 🚀 Fast and lightweight
- 🧵 Multi-threaded cleaning for better performance
- 🖥️ Native Windows API (WinAPI)
- 🧹 Cleans multiple system locations at once
- 🎨 Colored console output with clear status logs
- ❌ No external dependencies

---

## 🗂️ Cleaned Locations

The application automatically cleans the following directories:

- **User TEMP folder**
- **Windows TEMP**
- **Prefetch**
- **Windows Update Cache**
- **Windows Error Reports (WER)**

All files and subfolders inside these directories are removed where access is permitted.

---

## 🛠️ How It Works

- Detects the user TEMP directory using environment variables
- Uses `std::filesystem` to iterate and remove files
- Runs cleaning tasks in parallel using `std::thread`
- Displays real-time status with colored logs:
  - ✅ DONE
  - ❌ ACCESS DENIED

---

## ⚠️ Notes

Some folders may require administrator privileges

Files that are locked or protected will be skipped automatically

The program is safe — it only targets well-known temporary system locations

--- 

## 📜 License

This project is licensed under GNU General Public License v3.0 (GPL-3.0).

---

