# Student Marks Management System

A Data Structures (DSA) mini project: a C++ console program for managing student marks, plus a browser dashboard that visualises the same logic.

**Author:** Shahu R Nikam, S.Y. IT-C, AISSMS Institute of Information Technology, Pune

## What is in this repo

| File | Purpose |
|------|---------|
| `student_marks.cpp` | The main project: menu-driven console program |
| `index.html` | Interactive dashboard (single file, no build step) |

## Data structures used

| Feature | Data structure | Key operations |
|---------|----------------|----------------|
| Student records | Singly linked list | Insert at head O(1), search by roll O(n), delete O(n) |
| Undo last marks update | Stack (array, size 100) | push / pop O(1) |
| Result requests (Marksheet, Verification) | Circular queue (array, size 50) | enqueue / dequeue O(1), FIFO |
| Merit list | Array + bubble sort | O(n²) |

## Features

- Add, display, search, update and delete students (5 subjects, 100 marks each)
- Grades A to F; any subject below 35 means FAIL
- Class statistics: average percentage, pass and fail counts, topper
- Merit list ranked by total marks
- Undo for the most recent marks update
- Queue of result requests processed in first-in, first-out order

## Run the C++ program

```bash
g++ -o student_marks student_marks.cpp
./student_marks
```

## Open the dashboard

Open `index.html` in any browser. It loads sample students so every panel has data; changes are kept in memory and reset on reload. It uses Google Fonts, so the fonts need an internet connection (it falls back to system fonts offline).

To host it on GitHub Pages: repository **Settings → Pages → Deploy from a branch → `main` / root**.

## Push to GitHub

```bash
git init
git add .
git commit -m "Add Student Marks Management System"
git branch -M main
git remote add origin https://github.com/<your-username>/student-marks-management-system.git
git push -u origin main
```
