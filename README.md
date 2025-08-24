📘 Lexical Analyzer Project  
A **Lexical Analyzer (Lexer/Scanner)** implemented in **C**.  
This project scans a given C source file, breaks it into tokens, and categorizes them into distinct types, including keywords, identifiers, operators, delimiters, numbers, string/character literals, and comments. It also generates a summary report and stores results in an output file.  

---

📚 Table of Contents  
- Features    
- Code Overview  
- Example  
- Limitations  
- License  
- Author  

---

🚀 Features  
🔹 Tokenizes C source code into:  
- Keywords  
- Identifiers  
- Operators  
- Integers & Real Numbers  
- String & Character Literals  
- Delimiters  
- Preprocessor Directives  
- Single-line & Multi-line Comments  
- Hex/Octal Numbers  

🔹 Generates a **summary report** with token counts  
🔹 Writes output to `output.txt`  
🔹 Demonstrates file handling and basic compiler design principles  

---

🧠 Code Overview

main.c → Main program with file handling and analyzer logic

header.h → Function declarations & structures

function.c → Functions for tokenizing input code

output.txt → Tokenized results + summary

---

⚠️ Limitations

* Designed for C source code only

* Doesn’t handle macros or complex preprocessor logic

* Limited error handling for malformed input

---

📄 License
This project is licensed under the MIT License.

👤 Author Jaya Karuensh. S
