// C program for student registration and management system

#include <stdio.h>  // To use printf and scanf
#include <string.h> // To use string functions
#include <stdlib.h> // To use malloc and free
#include <ctype.h>  // To use isalpha and other character functions

#define MAX_NAME_LENGTH 50
#define MAX_STUDENTS 100

// Structure to hold student information
// With name, Id, date of birth, email, 1st grade, 2nd grade, average and status
typedef struct
{
    char name[MAX_NAME_LENGTH];
    int id;
    char dob[11]; // Format: YYYY-MM-DD
    char email[MAX_NAME_LENGTH];
    float firstGrade;
    float secondGrade;
    float average;
    char status[20]; // e.g., "Passed", "Failed", "Retaken"
} Student;

//Function to display menu
void displayMenu(int studentCount)
{
    printf("==============================\n");
    printf("Student Registration System\n");
    printf("Total students registered   : %d\n", studentCount);
    printf("==============================\n");
    printf("1. Register Student\n");
    printf("2. Display all Students\n");
    printf("3. Search Student by ID\n");
    printf("4. Filter Students by Status\n");
    printf("5. Edit Student Information\n");
    printf("6. Delete Student by ID\n");
    printf("7. Sort Students\n");
    printf("8. Filter Students by Average Range\n");
    printf("9. Count Students by Status\n");
    printf("10. Export Report to CSV\n");
    printf("11. Advanced Report\n");
    printf("12. Export Report to HTML\n");
    printf("13. Exit\n");
    printf("Enter your choice: ");
}

//Function to save students to file
void saveStudents(const Student *students, int studentCount)
{
    FILE *f = fopen("./students.csv", "w");
    if (!f)
    {
        printf("Erro ao salvar.\n");
        return;
    }
    for (int i = 0; i < studentCount; i++)
    {
        fprintf(f, "%d;%s;%s;%s;%.2f;%.2f;%.2f;%s\n",
                students[i].id, students[i].name, students[i].dob, students[i].email,
                students[i].firstGrade, students[i].secondGrade,
                students[i].average, students[i].status);
    }
    fclose(f);
}

//Function to load students from file
int loadStudents(Student *students)
{
    FILE *f = fopen("./students.csv", "r");
    if (!f)
        return 0;
    int count = 0;
    while (fscanf(f, "%d;%49[^;];%10[^;];%49[^;];%f;%f;%f;%19[^\n]\n",
                  &students[count].id, students[count].name, students[count].dob, students[count].email,
                  &students[count].firstGrade, &students[count].secondGrade,
                  &students[count].average, students[count].status) == 8)
    {
        count++;
    }
    fclose(f);
    return count;
}

//Function to pause and wait for user input
void pauseAndReturn()
{
    printf("\nPressione Enter para voltar ao menu...");
    while (getchar() != '\n')
        ; // wait for Enter
}

//Function to read and validate grade input
float readGrade(const char *label)
{
    float grade;
    while (1)
    {
        printf("%s (0 a 10): ", label);

        if (scanf("%f", &grade) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // clear buffer
        }
        else if (grade < 0.0 || grade > 10.0)
        {
            printf("Grade must be between 0 and 10.\n");
        }
        else
        {
            while (getchar() != '\n')
                ; // clear leftover \n
            return grade;
        }
    }
}

//Function to check if ID already exists
int idExists(const Student *students, int studentCount, int id)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == id)
            return 1;
    }
    return 0;
}

//Function to validate email format
int isValidEmail(const char *email)
{
    const char *at = strchr(email, '@');
    if (!at || at == email || strchr(at + 1, '@'))
        return 0;
    const char *dot = strrchr(at, '.');
    if (!dot || dot <= at + 1 || dot == email + strlen(email) - 1)
        return 0;
    return 1;
}

//Function to disregard case sensitivity in status comparison
void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

//Function to calculate average and determine status
void calculateAverageAndStatus(Student *student)
{
    student->average = (student->firstGrade + student->secondGrade) / 2.0;
    if (student->average >= 6.0)
    {
        strncpy(student->status, "Passed", sizeof(student->status) - 1);
        student->status[sizeof(student->status) - 1] = '\0';
        toLowerCase(student->status);
    }
    else if (student->average >= 4.0)
    {
        strncpy(student->status, "Retaken", sizeof(student->status) - 1);
        student->status[sizeof(student->status) - 1] = '\0';
        toLowerCase(student->status);
    }
    else
    {
        strncpy(student->status, "Failed", sizeof(student->status) - 1);
        student->status[sizeof(student->status) - 1] = '\0';
        toLowerCase(student->status);
    }
}

//Function to register a student
void registerStudent(Student *students, int *studentCount)
{
    if (*studentCount >= MAX_STUDENTS)
    {
        printf("Maximum student limit reached.\n");
        return;
    }
    Student *student = &students[*studentCount];
    printf("Enter name: ");
    scanf(" %[^\n]", student->name);
    do
    {
        printf("Enter ID: ");
        scanf("%d", &student->id);
        getchar(); //Clear newline

        if (idExists(students, *studentCount, student->id))
        {
            printf("This ID already exists. Please enter a unique ID.\n");
        }
    } while (idExists(students, *studentCount, student->id));
    printf("Enter date of birth (YYYY-MM-DD): ");
    scanf(" %[^\n]", student->dob);
    do
    {
        printf("Enter email: ");
        scanf(" %[^\n]", student->email);
        if (!isValidEmail(student->email))
        {
            printf("Invalid email format. Please enter a valid email.\n");
        }
    } while (!isValidEmail(student->email));
    student->firstGrade = readGrade("Enter first grade");
    student->secondGrade = readGrade("Enter second grade");
    calculateAverageAndStatus(student);
    (*studentCount)++;
    printf("Student registered successfully!\n");
    saveStudents(students, *studentCount);
}

//Function to display all registered students
void displayAllStudents(const Student *students, int studentCount)
{
    if (studentCount == 0)
    {
        printf("No students registered.\n");
        return;
    }
    printf("Registered Students:\n");
    for (int i = 0; i < studentCount; i++)
    {
        const Student *student = &students[i];
        printf("Name: %s, ID: %d, DOB: %s, Email: %s, First Grade: %.2f, Second Grade: %.2f, Average: %.2f, Status: %s\n",
               student->name, student->id, student->dob, student->email,
               student->firstGrade, student->secondGrade, student->average, student->status);
    }
}

//Function to search student by ID
Student *searchStudentByID(const Student *students, int studentCount, int id)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == id)
        {
            return (Student *)&students[i];
        }
    }
    return NULL;
}

// Function to filter students by status
void filterStudentsByStatus(const Student *students, int studentCount, const char *statusInput)
{
    char status[20];
    strncpy(status, statusInput, sizeof(status) - 1);
    status[sizeof(status) - 1] = '\0'; // copy the entered text
    toLowerCase(status);               // change to lower case

    printf("Students with status '%s':\n", statusInput);
    int found = 0;

    for (int i = 0; i < studentCount; i++)
    {
        char studentStatus[20];
        strncpy(studentStatus, students[i].status, sizeof(studentStatus) - 1);
        studentStatus[sizeof(studentStatus) - 1] = '\0';
        toLowerCase(studentStatus);

        if (strcmp(studentStatus, status) == 0)
        {
            printf("Name: %s, ID: %d, DOB: %s, Email: %s, First Grade: %.2f, Second Grade: %.2f, Average: %.2f\n",
                   students[i].name, students[i].id, students[i].dob, students[i].email,
                   students[i].firstGrade, students[i].secondGrade, students[i].average);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No students found with status '%s'.\n", statusInput);
    }
}

//Function to edit student information
void editStudent(Student *students, int studentCount, int id)
{
    Student *student = searchStudentByID(students, studentCount, id);
    if (!student)
    {
        printf("Student with ID %d not found.\n", id);
        return;
    }

    printf("Editing student %s (ID %d)\n", student->name, student->id);

    printf("Enter new name (or press Enter to keep '%s'): ", student->name);
    char buffer[MAX_NAME_LENGTH];
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n')
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // remove \n
        strncpy(student->name, buffer, MAX_NAME_LENGTH);
    }

    printf("Enter new date of birth (YYYY-MM-DD) or Enter to keep '%s': ", student->dob);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n')
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(student->dob, buffer, sizeof(student->dob));
    }

    printf("Enter new email or Enter to keep '%s': ", student->email);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n')
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (isValidEmail(buffer))
        {
            strncpy(student->email, buffer, MAX_NAME_LENGTH);
        }
        else
        {
            printf("Invalid email, keeping old one.\n");
        }
    }

    printf("Enter new first grade (or press Enter to keep %.2f): ", student->firstGrade);
    char gradeBuffer[20];
    fgets(gradeBuffer, sizeof(gradeBuffer), stdin);
    if (gradeBuffer[0] != '\n')
    {
        float g1 = strtof(gradeBuffer, NULL);
        if (g1 >= 0.0 && g1 <= 10.0)
        {
            student->firstGrade = g1;
        }
        else
        {
            printf("Invalid grade, keeping old one.\n");
        }
    }

    printf("Enter new second grade (or press Enter to keep %.2f): ", student->secondGrade);
    fgets(gradeBuffer, sizeof(gradeBuffer), stdin);
    if (gradeBuffer[0] != '\n')
    {
        float g2 = strtof(gradeBuffer, NULL);
        if (g2 >= 0.0 && g2 <= 10.0)
        {
            student->secondGrade = g2;
        }
        else
        {
            printf("Invalid grade, keeping old one.\n");
        }
    }

    calculateAverageAndStatus(student);
    printf("Student updated successfully!\n");
}

// Function to delete a student by ID
void deleteStudent(Student *students, int *studentCount, int id)
{
    int index = -1;
    for (int i = 0; i < *studentCount; i++)
    {
        if (students[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Student with ID %d not found.\n", id);
        return;
    }

    // Shift students to fill the gap
    for (int i = index; i < *studentCount - 1; i++)
    {
        students[i] = students[i + 1];
    }
    (*studentCount)--;

    printf("Student with ID %d deleted successfully!\n", id);
}

// Comparison functions for sorting
int compareByAverage(const void *a, const void *b)
{
    float avgA = ((Student *)a)->average;
    float avgB = ((Student *)b)->average;
    return (avgA > avgB) - (avgA < avgB);
}

// Comparison functions for sorting
int compareByName(const void *a, const void *b)
{
    return strcasecmp(((Student *)a)->name, ((Student *)b)->name);
}

int compareByID(const void *a, const void *b)
{
    return ((Student *)a)->id - ((Student *)b)->id;
}

void filterByAverageRange(const Student *students, int studentCount, float min, float max)
{
    printf("Students with average between %.2f and %.2f:\n", min, max);
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].average >= min && students[i].average <= max)
        {
            printf("Name: %s, ID: %d, Average: %.2f\n", students[i].name, students[i].id, students[i].average);
        }
    }
}

// Function to count students by status
void countByStatus(const Student *students, int studentCount)
{
    int passed = 0, failed = 0, retaken = 0;
    for (int i = 0; i < studentCount; i++)
    {
        if (strcmp(students[i].status, "passed") == 0)
            passed++;
        else if (strcmp(students[i].status, "failed") == 0)
            failed++;
        else if (strcmp(students[i].status, "retaken") == 0)
            retaken++;
    }
    printf("Passed: %d, Retaken: %d, Failed: %d\n", passed, retaken, failed);
}

// Function to export report to CSV
void exportReport(const Student *students, int studentCount)
{
    FILE *f = fopen("./report.csv", "w");
    if (!f)
        return;
    fprintf(f, "ID;Name;DOB;Email;Average;Status\n");
    for (int i = 0; i < studentCount; i++)
    {
        fprintf(f, "%d;%s;%s;%s;%.2f;%s\n",
                students[i].id, students[i].name, students[i].dob, students[i].email,
                students[i].average, students[i].status);
    }
    fclose(f);
    printf("Report exported to report.csv\n");
}

void advancedReport(const Student *students, int studentCount)
{
    if (studentCount == 0)
    {
        printf("No students registered.\n");
        return;
    }

    // Average of the class
    float total = 0.0;
    for (int i = 0; i < studentCount; i++)
    {
        total += students[i].average;
    }
    float classAverage = total / studentCount;

    // Best and worst student
    const Student *best = &students[0];
    const Student *worst = &students[0];
    for (int i = 1; i < studentCount; i++)
    {
        if (students[i].average > best->average)
            best = &students[i];
        if (students[i].average < worst->average)
            worst = &students[i];
    }

    // Distribution of averages by ranges
    int faixa0_4 = 0, faixa4_6 = 0, faixa6_8 = 0, faixa8_10 = 0;
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].average < 4.0)
            faixa0_4++;
        else if (students[i].average < 6.0)
            faixa4_6++;
        else if (students[i].average < 8.0)
            faixa6_8++;
        else
            faixa8_10++;
    }

    // Display report
    printf("\n===== Advanced Report =====\n");
    printf("Class average: %.2f\n", classAverage);
    printf("Best student: %s (ID %d) with average %.2f\n",
           best->name, best->id, best->average);
    printf("Worst student: %s (ID %d) with average %.2f\n",
           worst->name, worst->id, worst->average);

    printf("\nDistribution of averages:\n");
    printf("0.0 - 3.9 : %d students\n", faixa0_4);
    printf("4.0 - 5.9 : %d students\n", faixa4_6);
    printf("6.0 - 7.9 : %d students\n", faixa6_8);
    printf("8.0 - 10.0: %d students\n", faixa8_10);
    printf("===========================\n");
}

// Function to export HTML report
void exportHTML(const Student *students, int studentCount)
{
    FILE *f = fopen("./report.html", "w");
    if (!f)
    {
        printf("Erro ao criar report.html\n");
        return;
    }

    fprintf(f,
            "<!DOCTYPE html>\n"
            "<html lang=\"pt\">\n"
            "<head>\n"
            "  <meta charset=\"UTF-8\" />\n"
            "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n"
            "  <title>Relatório de Alunos</title>\n"
            "  <style>\n"
            "    :root { --bg:#0f172a; --card:#111827; --muted:#94a3b8; --text:#e5e7eb; --accent:#22c55e; --warn:#ef4444; --retake:#f59e0b; }\n"
            "    body { background:var(--bg); color:var(--text); font-family: system-ui, -apple-system, Segoe UI, Roboto, Ubuntu, Cantarell, 'Helvetica Neue', Arial, 'Noto Sans', sans-serif; margin:0; padding:2rem; }\n"
            "    .container { max-width:1100px; margin:0 auto; }\n"
            "    h1 { margin:0 0 1rem; font-weight:700; }\n"
            "    .meta { display:flex; gap:1rem; flex-wrap:wrap; margin-bottom:1rem; color:var(--muted); }\n"
            "    .meta > div { background:var(--card); padding:.6rem .8rem; border-radius:.6rem; border:1px solid #1f2937; }\n"
            "    .controls { background:var(--card); border:1px solid #1f2937; padding:1rem; border-radius:.8rem; margin:1rem 0; display:grid; grid-template-columns: repeat(4, minmax(180px,1fr)); gap:.8rem; }\n"
            "    .controls label { display:block; font-size:.85rem; color:var(--muted); margin-bottom:.3rem; }\n"
            "    .controls input, .controls select { width:100%; padding:.55rem .6rem; border-radius:.5rem; border:1px solid #374151; background:#0b1220; color:var(--text); }\n"
            "    .controls button { padding:.6rem .9rem; border-radius:.5rem; border:1px solid #374151; background:#0b1220; color:var(--text); cursor:pointer; }\n"
            "    .controls button:hover { background:#0e1628; }\n"
            "    table { width:100%; border-collapse: collapse; background:var(--card); border:1px solid #1f2937; border-radius:.8rem; overflow:hidden; }\n"
            "    thead th { text-align:left; font-weight:600; color:var(--muted); padding:.8rem; background:#0b1220; border-bottom:1px solid #1f2937; cursor:pointer; }\n"
            "    tbody td { padding:.8rem; border-bottom:1px solid #1f2937; }\n"
            "    tbody tr:hover { background:#0e1628; }\n"
            "    .status { padding:.25rem .45rem; border-radius:.4rem; font-size:.85rem; display:inline-block; }\n"
            "    .passed { color:#10b981; background:#064e3b; border:1px solid #065f46; }\n"
            "    .retaken { color:#f59e0b; background:#3f2d0b; border:1px solid #92400e; }\n"
            "    .failed { color:#ef4444; background:#3f0d0d; border:1px solid #7f1d1d; }\n"
            "    .footer { color:var(--muted); font-size:.85rem; margin-top:1rem; }\n"
            "  </style>\n"
            "</head>\n"
            "<body>\n"
            "<div class=\"container\">\n"
            "  <h1>Relatório de Alunos</h1>\n"
            "  <div class=\"meta\">\n"
            "    <div id=\"total\">Total: %d</div>\n",
            studentCount);

    //Statistics
    int passed = 0, failed = 0, retaken = 0;
    float sum = 0.0f, maxAvg = -1.0f, minAvg = 11.0f;
    int bestIdx = 0, worstIdx = 0;
    for (int i = 0; i < studentCount; i++)
    {
        sum += students[i].average;
        if (students[i].average > maxAvg)
        {
            maxAvg = students[i].average;
            bestIdx = i;
        }
        if (students[i].average < minAvg)
        {
            minAvg = students[i].average;
            worstIdx = i;
        }
        if (strcasecmp(students[i].status, "passed") == 0)
            passed++;
        else if (strcasecmp(students[i].status, "failed") == 0)
            failed++;
        else if (strcasecmp(students[i].status, "retaken") == 0)
            retaken++;
    }
    float classAvg = studentCount ? sum / studentCount : 0.0f;

    fprintf(f,
            "    <div>Turma: média %.2f</div>\n"
            "    <div>Melhor: %s (%.2f)</div>\n"
            "    <div>Pior: %s (%.2f)</div>\n"
            "    <div>Aprovados: %d</div>\n"
            "    <div>Em recuperação: %d</div>\n"
            "    <div>Reprovados: %d</div>\n"
            "  </div>\n",
            classAvg,
            students[bestIdx].name, students[bestIdx].average,
            students[worstIdx].name, students[worstIdx].average,
            passed, retaken, failed);

    //Control panel
    fprintf(f,
            "  <div class=\"controls\">\n"
            "    <div><label>Pesquisar por nome</label><input id=\"q\" type=\"text\" placeholder=\"ex: Maria\" /></div>\n"
            "    <div><label>Status</label><select id=\"status\"><option value=\"\">Todos</option><option value=\"passed\">Aprovado</option><option value=\"retaken\">Recuperação</option><option value=\"failed\">Reprovado</option></select></div>\n"
            "    <div><label>Média mínima</label><input id=\"min\" type=\"number\" min=\"0\" max=\"10\" step=\"0.1\" /></div>\n"
            "    <div><label>Média máxima</label><input id=\"max\" type=\"number\" min=\"0\" max=\"10\" step=\"0.1\" /></div>\n"
            "    <div><button id=\"reset\">Limpar filtros</button></div>\n"
            "  </div>\n");

    //Table
    fprintf(f,
            "  <table id=\"tbl\">\n"
            "    <thead>\n"
            "      <tr>\n"
            "        <th data-key=\"id\">ID</th>\n"
            "        <th data-key=\"name\">Nome</th>\n"
            "        <th>Data de nascimento</th>\n"
            "        <th>Email</th>\n"
            "        <th data-key=\"average\">Média</th>\n"
            "        <th data-key=\"status\">Status</th>\n"
            "      </tr>\n"
            "    </thead>\n"
            "    <tbody>\n");

    //Rows
    for (int i = 0; i < studentCount; i++)
    {
        const char *statusClass =
            (strcasecmp(students[i].status, "passed") == 0) ? "passed" : (strcasecmp(students[i].status, "retaken") == 0) ? "retaken"
                                                                                                                          : "failed";

        fprintf(f,
                "      <tr>\n"
                "        <td>%d</td>\n"
                "        <td>%s</td>\n"
                "        <td>%s</td>\n"
                "        <td>%s</td>\n"
                "        <td>%.2f</td>\n"
                "        <td><span class=\"status %s\">%s</span></td>\n"
                "      </tr>\n",
                students[i].id,
                students[i].name,
                students[i].dob,
                students[i].email,
                students[i].average,
                statusClass,
                students[i].status);
    }

    //JavaScript
    fprintf(f,
            "    </tbody>\n"
            "  </table>\n"
            "  <div class=\"footer\">Gerado automaticamente pelo sistema de registro.</div>\n"
            "</div>\n"
            "<script>\n"
            "const tbl = document.getElementById('tbl');\n"
            "const q = document.getElementById('q');\n"
            "const status = document.getElementById('status');\n"
            "const minI = document.getElementById('min');\n"
            "const maxI = document.getElementById('max');\n"
            "const resetBtn = document.getElementById('reset');\n"
            "\n"
            // Sorting by headers
            "let sortKey = null, sortDir = 1;\n"
            "tbl.querySelectorAll('thead th[data-key]').forEach(th => th.addEventListener('click', () => {\n"
            "  const key = th.dataset.key;\n"
            "  sortDir = (sortKey === key) ? -sortDir : 1;\n"
            "  sortKey = key;\n"
            "  const rows = Array.from(tbl.tBodies[0].rows);\n"
            "  rows.sort((a,b) => {\n"
            "    const ia = (key==='id'||key==='average') ? parseFloat(a.cells[key==='id'?0:4].textContent) : a.cells[key==='name'?1:5].textContent.toLowerCase();\n"
            "    const ib = (key==='id'||key==='average') ? parseFloat(b.cells[key==='id'?0:4].textContent) : b.cells[key==='name'?1:5].textContent.toLowerCase();\n"
            "    if (ia < ib) return -1*sortDir; if (ia > ib) return 1*sortDir; return 0;\n"
            "  });\n"
            "  tbl.tBodies[0].append(...rows);\n"
            "}));\n"
            "\n"
            // Filtering
            "function applyFilters(){\n"
            "  const qv = q.value.trim().toLowerCase();\n"
            "  const sv = status.value;\n"
            "  const min = parseFloat(minI.value); const max = parseFloat(maxI.value);\n"
            "  let passed=0, failed=0, retaken=0, total=0;\n"
            "  Array.from(tbl.tBodies[0].rows).forEach(r => {\n"
            "    const name = r.cells[1].textContent.toLowerCase();\n"
            "    const avg = parseFloat(r.cells[4].textContent);\n"
            "    const st = r.cells[5].textContent.trim().toLowerCase();\n"
            "    let ok = true;\n"
            "    if (qv && !name.includes(qv)) ok = false;\n"
            "    if (sv && st !== sv) ok = false;\n"
            "    if (!isNaN(min) && avg < min) ok = false;\n"
            "    if (!isNaN(max) && avg > max) ok = false;\n"
            "    r.style.display = ok ? '' : 'none';\n"
            "    if (ok){ total++; if (st==='passed') passed++; else if (st==='failed') failed++; else retaken++; }\n"
            "  });\n"
            "  document.getElementById('total').textContent = `Total: ${total}`;\n"
            "}\n"
            "[q,status,minI,maxI].forEach(el => el.addEventListener('input', applyFilters));\n"
            "resetBtn.addEventListener('click', () => { q.value=''; status.value=''; minI.value=''; maxI.value=''; applyFilters(); });\n"
            "applyFilters();\n"
            "</script>\n"
            "</body>\n"
            "</html>\n");

    fclose(f);
    printf("HTML exportado para report.html\n");
}

// Main function
int main()
{
    Student students[MAX_STUDENTS];
    int studentCount = loadStudents(students);
    int choice;

    while (1)
    {
        displayMenu(studentCount);
        scanf("%d", &choice);
        getchar(); // Clear newline character from input buffer
        switch (choice)
        {
        case 1:
            registerStudent(students, &studentCount);
            pauseAndReturn();
            break;
        case 2:
            displayAllStudents(students, studentCount);
            pauseAndReturn();
            break;
        case 3:
        {
            int id;
            printf("Enter student ID to search: ");
            scanf("%d", &id);
            Student *student = searchStudentByID(students, studentCount, id);
            if (student)
            {
                printf("Student found: Name: %s, ID: %d, DOB: %s, Email: %s, First Grade: %.2f, Second Grade: %.2f, Average: %.2f, Status: %s\n",
                       student->name, student->id, student->dob, student->email,
                       student->firstGrade, student->secondGrade, student->average, student->status);
            }
            else
            {
                printf("Student with ID %d not found.\n", id);
            }
            pauseAndReturn();
        }
            break;
        case 4:
        {
            char status[20];
            printf("Enter status to filter by (Passed, Retaken, Failed): ");
            scanf(" %[^\n]", status);
            filterStudentsByStatus(students, studentCount, status);
            pauseAndReturn();
        }
            break;
        case 5:
        {
            int id;
            printf("Enter student ID to edit: ");
            scanf("%d", &id);
            getchar(); // Clear newline
            editStudent(students, studentCount, id);
            saveStudents(students, studentCount);
            pauseAndReturn();
        }
            break;
        case 6:
        {
            int id;
            printf("Enter student ID to delete: ");
            scanf("%d", &id);
            getchar(); // Clear newline
            deleteStudent(students, &studentCount, id);
            saveStudents(students, studentCount);
            pauseAndReturn();
        }
            break;
        case 7:
        {
            int sortChoice;
            printf("Sort by: 1. Average 2. Name 3. ID: ");
            scanf("%d", &sortChoice);
            switch (sortChoice)
            {
            case 1:
                qsort(students, studentCount, sizeof(Student), compareByAverage);
                break;
            case 2:
                qsort(students, studentCount, sizeof(Student), compareByName);
                break;
            case 3:
                qsort(students, studentCount, sizeof(Student), compareByID);
                break;
            default:
                printf("Invalid sort choice.\n");
                break;
            }
            printf("Students sorted successfully.\n");
            pauseAndReturn();
        }
            break;
        case 8:
        {
            float min, max;
            printf("Enter minimum average: ");
            scanf("%f", &min);
            printf("Enter maximum average: ");
            scanf("%f", &max);
            filterByAverageRange(students, studentCount, min, max);
            pauseAndReturn();
        }
            break;
        case 9:
            countByStatus(students, studentCount);
            pauseAndReturn();
            break;
        case 10:
            exportReport(students, studentCount);
            pauseAndReturn();
            break;
        case 11:
            advancedReport(students, studentCount);
            pauseAndReturn();
            break;
        case 12:
            exportHTML(students, studentCount);
            pauseAndReturn();
            break;
        case 13:
        {
            printf("Exiting the program.\n");
            return 0;
        }
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
