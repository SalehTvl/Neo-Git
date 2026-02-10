# Neo-Git: A Simple Git Simulator in C

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%2FmacOS%2FWindows-lightgrey.svg)

## 📄 Overview

Neo-Git is a command-line version control system developed as a project for the **Fundamentals of Programming** course. It emulates the basic functionalities of Git, providing a hands-on understanding of how version control works at a fundamental level. The entire application is written in **standard C**, focusing on direct file system manipulation and low-level I/O operations to manage versioning, staging, and commits.

Unlike the real Git which uses complex data structures like Merkle trees, Neo-Git adopts a straightforward approach: versioning is achieved by creating snapshots (full copies) of files for each commit, and metadata is stored in simple `.txt` log files. This design choice makes the underlying mechanics of version control transparent and easy to understand.

## ✨ Implemented Commands

Neo-Git supports a core subset of Git commands:

*   `neo-git init`: Initializes a new `.neogit` repository in the current directory.
*   `neo-git config`: Sets user configuration like username and email.
*   `neo-git add <file/dir>`: Adds files to the staging area. Supports adding by depth with `-d`.
*   `neo-git reset <file/dir>`: Removes files from the staging area.
*   `neo-git commit -m <message>`: Creates a new commit with the staged files, assigning it a unique ID and timestamp.
*   `neo-git log`: Displays the history of all commits made.
*   `neo-git status`: Shows the status of tracked, untracked, and modified files.
*   `neo-git checkout <commit-id/branch-name>`: Reverts the project files to a previous commit or switches to a branch.
*   `neo-git branch <branch-name>`: Creates a new branch.

## ⚙️ Technical Implementation

*   **Repository Structure:** When initialized, a hidden `.neogit` directory is created. This directory houses all metadata, including:
    *   A `config.txt` file for user details.
    *   A `staging.txt` file to track staged files.
    *   A `commits` directory where each commit is stored in a separate sub-directory.
    *   A `log.txt` file to record all commit messages, IDs, and authors.
*   **Versioning:** On commit, Neo-Git copies the entire content of every staged file into a new folder named after the commit ID. This "snapshot" approach simplifies the versioning logic.
*   **State Management:** The state of the working directory is determined by comparing file timestamps and content with the latest commit snapshot, allowing `status` to detect modified or new files.

## 🚀 Getting Started

### Prerequisites
*   A C compiler (like `gcc`).

### Installation & Run

1.  **Clone the repository:**
```bash
git clone https://github.com/SalehTvl/Neo-Git.git
cd Neo-Git
```

2. **Compile the source code:**
```bash
gcc -o neogit *.c
```

3. **Run Neo-Git:**
```bash
neogit <command> [arguments]
```
