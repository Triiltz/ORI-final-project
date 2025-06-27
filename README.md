# Tweet Search Engine - ORI Final Project

This repository contains my final project for the ORI (Information Organization and Retrieval) course.  
The project is a **search engine for posts in a corpus of Tweets**, written in **C**, using data structures such as **AVL Trees**, **Hash Tables**, and **Sets** to support efficient keyword-based retrieval.

---

## 🧠 Project Description

The goal is to create a command-line tool that:

- Loads a `.csv` file containing multiple tweet-like posts.
- Indexes the words from each post using a **Hash Table**.
- Allows the user to perform **boolean searches** using `AND`, `OR`, and `NOT`.
- Displays the list of matching tweets based on the given query.
- Uses AVL Trees and Sets to efficiently manage and combine query results.

### 📄 Input Format

Each line in the CSV file (`corpus.csv`) is a tweet, in the format:
```
id,ignored,text
```
Example:
```
14,0, jb isnt showing in australia any more!
```

---

## ⚙️ Technologies & Data Structures Used

- `Hash Table` – indexes each word with the IDs of posts that contain it.
- `Set` – represents the result sets for search queries (internally uses AVL trees).
- `AVL Tree` – guarantees O(log n) operations in Set structures.
- `CSV Parsing` – for reading and displaying results from the tweet corpus.

---

## 🔎 Query Syntax

The user can enter queries like:
```
(i AND want)
(brazil OR carnival)
(winner NOT worldcup)
```

All terms are case-insensitive and punctuation is ignored during indexing.

---

## 🗂️ Project Structure

```
.
├── main.c              # Program entry point and query interpreter
├── corpus.csv          # Input dataset (tweets) [not included here]
├── exporta.csv         # Exported word-ID index (generated)
│
├── Hash/
│   ├── TabelaHash.c    # Hash Table logic (indexing and word search)
│   └── TabelaHash.h
│
├── Set/
│   ├── Set.c           # Set operations using AVL trees
│   └── Set.h
│
├── AVL/
│   ├── ArvoreAVL.c     # AVL tree implementation
│   └── ArvoreAVL.h
```

---

## ▶️ How to Run

1. Make sure `corpus.csv` is in the project directory.
2. Compile the project using a C compiler (e.g., `gcc`):
```bash
gcc main.c Hash/TabelaHash.c Set/Set.c AVL/ArvoreAVL.c -o tweet_search
```
3. Run the executable:
```bash
./tweet_search
```

You will be prompted to input queries until you type `sair`.

---

## 📤 Output

The program will display the tweet texts matching your query and also export the indexed word-ID map to `exporta.csv`.

---

## 🧾 Notes

- All punctuation is removed and words are normalized before indexing.
- The system does not support nested boolean expressions.
- The focus of the implementation was on demonstrating modularization and usage of data structures covered in the ORI course.

---

## 📦 Large File Handling (Git LFS)

> ⚠️ The `corpus.csv` file is tracked using [Git LFS (Large File Storage)](https://git-lfs.github.com).

To properly clone and run this project, make sure you have Git LFS installed:

```bash
# One-time setup (if not yet installed)
git lfs install

# Clone the repository (LFS files will be downloaded automatically)
git clone <repository-url>
```

If you cloned the repository before installing Git LFS, run:

```bash
git lfs pull
```

This ensures the `corpus.csv` file is correctly downloaded and available.

---

> **Project guide (pt-br):** [Guide (PDF)](./pdf/guide.pdf)

---
