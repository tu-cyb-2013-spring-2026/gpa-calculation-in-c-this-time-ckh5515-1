#include <stdio.h>
#include <string.h>

#define MAX_COURSES 1000

typedef struct
{
    char grade;
    int hours;
    char semester[10];
    int year;
} Course;

/* Change one lowercase letter to uppercase. */
char make_upper(char ch)
{
    if (ch >= 'a' && ch <= 'z')
    {
        ch = ch - ('a' - 'A');
    }
    return ch;
}

/* Go through a word and make each letter uppercase. */
void string_upper(char str[])
{
    int i = 0;
    while (str[i] != '\0')
    {
        str[i] = make_upper(str[i]);
        i++;
    }
}

/* Convert a letter grade to grade points on a 4.0 scale. */
double grade_points(char grade)
{
    grade = make_upper(grade);

    if (grade == 'A')
        return 4.0;
    if (grade == 'B')
        return 3.0;
    if (grade == 'C')
        return 2.0;
    if (grade == 'D')
        return 1.0;
    if (grade == 'F')
        return 0.0;

    return -1.0;
}

/* Check if the semester is one of the allowed values. */
int valid_semester(char semester[])
{
    if (strcmp(semester, "SPRING") == 0)
        return 1;
    if (strcmp(semester, "SUMMER") == 0)
        return 1;
    if (strcmp(semester, "FALL") == 0)
        return 1;
    return 0;
}

int main()
{
    /* This array holds all valid classes the user enters. */
    Course courses[MAX_COURSES];
    char line[100];
    char again = 'Y';
    int count = 0;
    int i, j;

    printf("----> To quit entering grades, enter a blank line.\n");

    while (again == 'Y' || again == 'y')
    {
        /* Temporary variables for one class entry from the user. */
        char grade;
        int hours;
        char semester[10];
        int year;

        if (count >= MAX_COURSES)
        {
            printf("Maximum number of courses reached.\n");
            break;
        }

        printf("Enter: <Grade> <Hours> <Semester> <Year> (Example: A 3 FALL 2025): ");
        fgets(line, sizeof(line), stdin);

        if (line[0] == '\n')
        {
            break;
        }

        if (sscanf(line, " %c %d %9s %d", &grade, &hours, semester, &year) != 4)
        {
            printf("Invalid input. Please follow the example format.\n");
            continue;
        }

        /* Make grade and semester uppercase so input is case-insensitive. */
        grade = make_upper(grade);
        string_upper(semester);

        if (grade_points(grade) < 0)
        {
            printf("Invalid grade. Use A, B, C, D, or F.\n");
            continue;
        }

        if (hours <= 0)
        {
            printf("Invalid hours. Must be greater than 0.\n");
            continue;
        }

        if (!valid_semester(semester))
        {
            printf("Invalid semester. Choose from: SPRING, SUMMER, FALL.\n");
            continue;
        }

        if (year < 1900 || year > 2100)
        {
            printf("Invalid year. Enter a valid four-digit year.\n");
            continue;
        }

        /* Save this class now that all checks have passed. */
        courses[count].grade = grade;
        courses[count].hours = hours;
        strcpy(courses[count].semester, semester);
        courses[count].year = year;
        count++;

        printf("Would you like to add another class? (Y/N): ");
        scanf(" %c", &again);
        getchar(); /* Remove leftover Enter key from input buffer. */
    }

    printf("-------------------------------------\n");
    printf("Unofficial Transcript\n");

    if (count == 0)
    {
        printf("No course data entered.\n");
        printf("\n--\n");
        printf("Total credit hours: 0\n");
        printf("Overall GPA: 0.000\n");
        printf("-------------------------------------\n");
        return 0;
    }

    for (i = 0; i < count; i++)
    {
        int already_printed = 0;
        int semester_hours = 0;
        double semester_points = 0.0;

        /* If this term was already shown, skip it to avoid duplicates. */
        for (j = 0; j < i; j++)
        {
            if (strcmp(courses[i].semester, courses[j].semester) == 0 &&
                courses[i].year == courses[j].year)
            {
                already_printed = 1;
            }
        }

        if (already_printed)
            continue;

        /* Add up hours and points for this semester and year. */
        for (j = 0; j < count; j++)
        {
            if (strcmp(courses[i].semester, courses[j].semester) == 0 &&
                courses[i].year == courses[j].year)
            {
                semester_hours += courses[j].hours;
                semester_points += grade_points(courses[j].grade) * courses[j].hours;
            }
        }

        printf("%-6s %4d (%d hours) %.3f\n",
               courses[i].semester,
               courses[i].year,
               semester_hours,
               semester_points / semester_hours);
    }

    int total_hours = 0;
    double total_points = 0.0;

    /* Overall GPA = total quality points / total credit hours. */
    for (i = 0; i < count; i++)
    {
        total_hours += courses[i].hours;
        total_points += grade_points(courses[i].grade) * courses[i].hours;
    }

    printf("\n--\n");
    printf("Total credit hours: %d\n", total_hours);
    printf("Overall GPA: %.3f\n", total_points / total_hours);
    printf("-------------------------------------\n");

    return 0;
}
