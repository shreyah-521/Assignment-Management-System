# Assignment Management System

A simple Command Line Interface (CLI) application written in C to help students manage their homework and assignments. It allows you to save, view, search, edit, and track your tasks.

## Features

* **Add New Tasks:** Save assignments with a title, description, and due date.
* **Sort by Due Date:** Displays all active tasks automatically sorted from the earliest due date to the latest.
* **Search System:** Find tasks quickly using either the Assignment ID or words from the title.
* **Update and Delete:** Change details of existing tasks or remove them (soft delete).
* **Track Status:** Mark your assignments as "Pending" or "Completed".
* **Data Storage:** Saves all tasks into a binary file (`assignments.dat`), so your data is safe even after closing the program.

## How to Run the Project

### Prerequisites
You need a C compiler installed on your computer, such as `gcc`.

### Steps to Compile and Run
1. Download or clone this repository.
2. Open your terminal or command prompt in the project folder.
3. Compile the code using the following command:
   ```bash
   gcc main.c -o assignment_mgr
