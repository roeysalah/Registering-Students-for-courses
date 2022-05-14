

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structures
typedef struct student {
    char* name;
    int  id;
    struct clist* courses;
} student;

typedef struct course {
    char* title;
    int  number;
    struct slist* students;
} course;

typedef struct slist {
    student* info;
    struct slist* next;
} slist;

typedef struct clist {
    course* info;
    struct clist* next;
} clist;


// Different calls for allocate function
//#define allocate_def(m_size) allocate(m_size, NULL, 0)
//#define allocate_safe1(m_size, erace) allocate(m_size, (void*[]){ (void*)erace }, 1)
//#define allocate_safe2(m_size, erace_1, erace_2) allocate(m_size, (void*[]){ (void*)erace_1, (void*)erace_2 }, 2)


// prototypes
slist* add_student(slist* students, char* name, int id);
clist* add_course(clist* courses, char* title, int number);
void reg_student(slist* students, clist* courses, int id, int number);
void unreg_student(slist* students, int id, int number);
void free_all(slist* sl, clist* cl);
void print_students(slist* students);
void print_courses(clist* courses);


     /* Helpers */
// ------------------ //

// Constructors
slist* create_student(char* name, int id);
slist* sl_cpy_constructor(slist* sl);
clist* create_course(char* title, int number);
clist* cl_cpy_constructor(clist* cl);

// Interface
clist* find_course(clist* courses, int number);
slist* find_student(slist* students, int id);
slist* add_sl_node(slist* sl_nodes, slist* new_node);
clist* add_cl_node(clist* cl_nodes, clist* new_node);
clist* remove_course(clist* courses, clist* cl_remove);
slist* remove_student(slist* students, slist* sl_remove);

// Printers
void print_sl_cl(clist* courses);
void print_cl_sl(slist* students);

// Free
void free_courses(clist* cl, int all_info);
void free_students(slist* sl, int all_info);
void release_course(clist* cl);
void release_student(slist* sl);

// Allocaters
void* allocate(int size, void* erace1, void* erace2);


//DO NOT TOUCH THIS FUNCTION
static void getstring(char* buf, int length) {
    int len;
    buf = fgets(buf, length, stdin);
    len = (int)strlen(buf);
    if (buf[len - 1] == '\n')
        buf[len - 1] = '\0';
}

//DO NOT TOUCH THIS FUNCTION
int main() {
    slist* students = 0;
    clist* courses = 0;
    char  c;
    char  buf[100];
    int   id, num;

    do {
        printf("Choose:\n"
            "    add (s)tudent\n"
            "    add (c)ourse\n"
            "    (r)egister student\n"
            "    (u)nregister student\n"
            "    (p)rint lists\n"
            "    (q)uit\n");

        while ((c = (char)getchar()) == '\n');
        getchar();

        switch (c) {
        case 's':
            printf("Adding new student.\n");

            printf("Student name: ");
            getstring(buf, 100);

            printf("Student ID: ");
            scanf("%d", &id);

            students = add_student(students, buf, id);

            break;

        case 'c':
            printf("Adding new course.\n");

            printf("Course name: ");
            getstring(buf, 100);

            printf("Course number: ");
            scanf("%d", &num);

            courses = add_course(courses, buf, num);

            break;

            case 'r':
                printf("Registering a student to a course.\n");

                printf("Student ID: ");
                scanf("%d", &id);

                printf("Course number: ");
                scanf("%d", &num);

                reg_student(students, courses, id, num);

                break;

            case 'u':
                printf("unregistering a student from a course.\n");

                printf("student id: ");
                scanf("%d", &id);

                printf("course number: ");
                scanf("%d", &num);

                unreg_student(students, id, num);

                break;

        case 'p':
            printf("Printing Information.\n");

            print_students(students);
            print_courses(courses);

            break;

        case 'q':
            printf("Quitting...\n");
            break;

        }

        if (c != 'q')
            printf("\n");
    } while (c != 'q');

    free_all(students,courses);
    return 0;
}

slist* add_student(slist* students, char* name, int id) {
    slist* curr_student = NULL;
    slist* new_student = NULL;

    // In case student already exist
    if (find_student(students, id))
    {
        printf("Student id:%d already exist", id);
        return students;
    }

    // Adding new student to students
    new_student = create_student(name, id);
    students = add_sl_node(students, new_student);

    return students;
}


clist* add_course(clist* courses, char* title, int number) {

    clist* cur_course = NULL;
    clist* new_course = NULL;

    // Checking Wheather course id already exist
    if (find_course(courses, number))
    {
        printf("Courses id:%d already exist", number);
        return courses;
    }

    // Adding the course
    new_course = create_course(title, number);
    courses = add_cl_node(courses, new_course);

    return courses;
}

slist* create_student(char* name, int id)
{
    slist* st_lst = NULL;
    student* st = NULL;

    // Allocating student's memory
    st_lst = (slist*)allocate(sizeof(slist), NULL, NULL);
    st = (student*)allocate(sizeof(student), st_lst, NULL);
    st->name = (char*)allocate(sizeof(char) * (strlen(name) + 1), st_lst, st);

    // Initiating student
    st->id = id;
    strcpy(st->name, name);
    st->courses = NULL;

    // Referancing student to lst
    st_lst->info = st;
    st_lst->next = NULL;

    return st_lst;
}

slist* sl_cpy_constructor(slist* sl)
{
    slist* new_sl = NULL;

    // Shallow copy
    new_sl = (slist*)allocate(sizeof(slist), NULL, NULL);
    new_sl->info = sl->info;
    new_sl->next = NULL;

    return new_sl;
}

clist* create_course(char* title, int number)
{
    clist* cr_lst = NULL;
    course* cr = NULL;

    // Allocating
    cr_lst = (clist*)allocate(sizeof(clist), NULL, NULL);
    cr = (course*)allocate(sizeof(course), cr_lst, NULL);
    cr->title = (char*)allocate(sizeof(char) * (strlen(title) + 1), cr_lst, cr);

    // Initiating course
    strcpy(cr->title, title);
    cr->number = number;
    cr->students = NULL;

    // Referancing course to lst
    cr_lst->info = cr;
    cr_lst->next = NULL;

    return cr_lst;
}

clist* cl_cpy_constructor(clist* cl)
{
    clist* new_cl = NULL;

    // Shallow copy
    new_cl = (clist*)allocate(sizeof(clist), NULL, NULL);
    new_cl->info = cl->info;
    new_cl->next = NULL;

    return new_cl;
}

clist* find_course(clist* courses, int number)
{
    clist* cr = courses;

    // Searching the course
    while (cr != NULL && cr->info->number != number)
        cr = cr->next;

    return cr;
}

slist* find_student(slist* students, int id)
{
    slist* sr = students;

    // Searching the course
    while (sr != NULL && sr->info->id != id)
        sr = sr->next;

    return sr;
}

clist* remove_course(clist* courses, clist* cl_remove)
{
    clist* curr = NULL;

    if (cl_remove == courses)
    {
        // Deleting list's head (special case)
        courses = courses->next;
    }
    else
    {
        // Finding one node behind
        curr = courses;
        while (curr->next != cl_remove)
            curr = curr->next;

        // Deleting node from list
        curr->next = curr->next->next;
    }

    return courses;
}

slist* remove_student(slist* students, slist* sl_remove)
{
    slist* curr = NULL;

    if (students == sl_remove)
    {
        // Deleting list's head (special case)
        students = students->next;
    }
    else
    {
        // Finding one node behind
        curr = students;
        while (curr->next != sl_remove)
            curr = curr->next;

        // Deleting node from list
        curr->next = curr->next->next;
    }

    return students;
}


slist* add_sl_node(slist* sl_nodes, slist* new_node)
{
    slist* curr_student = NULL;

    // Sorted addition of node to nodes list
    if (sl_nodes == NULL)
    {
        sl_nodes = new_node;
    }
    else if (sl_nodes->info->id > new_node->info->id)
    {
        new_node->next = sl_nodes;
        sl_nodes = new_node;
    }
    else
    {
        curr_student = sl_nodes;
        while (curr_student->next && curr_student->next->info->id < new_node->info->id)
        {
            curr_student = curr_student->next;
        }

        new_node->next = curr_student->next;
        curr_student->next = new_node;
    }

    return sl_nodes;
}

clist* add_cl_node(clist* cl_nodes, clist* new_node)
{
    clist* curr_course = NULL;

    // Sorted addition of node to nodes list
    if (cl_nodes == NULL)
    {
        cl_nodes = new_node;
    }
    else if (cl_nodes->info->number > new_node->info->number)
    {
        new_node->next = cl_nodes;
        cl_nodes = new_node;
    }
    else
    {
        curr_course = cl_nodes;
        while (curr_course->next && curr_course->next->info->number < new_node->info->number)
        {
            curr_course = curr_course->next;
        }

        new_node->next = curr_course->next;
        curr_course->next = new_node;
    }

    return cl_nodes;
}

/*
This function allocates memory
Parameters:
    size   - amount of bytes memory to allocate
    erace1 - depended memory to erace in case of failure
    erace2 - depended memory to erace in case of failure

Return val:
    Returns void pointer to the memory allocated
*/
void* allocate(int size, void* erace1, void* erace2)
{
    void* memory = NULL;
    int i = 0;

    // Getting memory
    memory = malloc(size);

    // Releasing depended memory in case of failure
    if (memory == NULL)
    {
        // Freeing depended memory
        if (erace1)
            free(erace1);
        if (erace2)
            free(erace2);

        exit(1);
    }

    return memory;
}


void reg_student(slist* students, clist* courses, int id, int number)
{
    slist* st_to_reg = NULL;
    clist* cr_to_reg = NULL;

    // Getting student
    st_to_reg = find_student(students, id);
    if (st_to_reg == NULL)
        exit(1);

    // Getting course
    cr_to_reg = find_course(courses, number);
    if (cr_to_reg == NULL)
        exit(1);

    // Making the registration in case they both exist
    cr_to_reg->info->students = add_sl_node(cr_to_reg->info->students, sl_cpy_constructor(st_to_reg));
    st_to_reg->info->courses = add_cl_node(st_to_reg->info->courses, cl_cpy_constructor(cr_to_reg));
}

void unreg_student(slist* students, int id, int number)
{
    slist* sl_unreg = NULL;
    clist* cl_unreg = NULL;

    // Finding relevant course and student
    sl_unreg = find_student(students, id);
    if (sl_unreg)
    {
        cl_unreg = find_course(sl_unreg->info->courses, number);

        // If student isn't register for the chosen course -> exit
        if (!cl_unreg)
            exit(1);

        sl_unreg = find_student(cl_unreg->info->students, id);
    }

    if (sl_unreg && cl_unreg)
    {
        // Removing course from student's list and vise versa
        cl_unreg->info->students = remove_student(cl_unreg->info->students, sl_unreg);
        sl_unreg->info->courses = remove_course(sl_unreg->info->courses, cl_unreg);

        // Freeing memory
        free(cl_unreg);
        free(sl_unreg);
    }
    else
    {
        if (sl_unreg == NULL)
            printf("\nNo such student id:%d\n", id);
        else
            printf("\nStudent id:%d isn't register to course number:%d\n", id, number);
    }
}


void print_students(slist* students) {
    int i = 1;
    clist* courses = NULL;

    // PRinting headline
    printf("\nSTUDENT LIST:");
    if (students == NULL)
        printf("EMPTY");
    printf("\n");

    while (students != NULL) {
        printf("%d:%s", students->info->id, students->info->name);

        // Printing courses
        print_sl_cl(students->info->courses);

        students = students->next;
        i++;
    }
}

void print_sl_cl(clist* courses)
{
    // Printing wheather there are courses
    if (courses != NULL)
        printf("\ncourses: ");
    else
        printf("\nstudent is not registered for courses.");

    // Printing the courses
    while (courses != NULL)
    {
        printf("%d-%s:", courses->info->number, courses->info->title);
        if (courses->next != NULL)
            printf(", ");
        courses = courses->next;
    }
    printf("\n");
}

void print_courses(clist* courses)
{
    slist* st_curr = NULL;

    // Printing headline
    printf("\nCOURSES LIST:");
    if (courses == NULL)
        printf("EMPTY");
    printf("\n");

    while (courses != NULL) {
        printf("%d:%s", courses->info->number, courses->info->title);

        // Printing Students
        print_cl_sl(courses->info->students);

        courses = courses->next;
    }
}

void print_cl_sl(slist* students)
{
    // Printing wheather there are students
    if (students != NULL)
        printf("\nstudents: ");
    else
        printf("\ncourse has no students.");

    // Printing the students
    while (students != NULL)
    {
        printf("%d-%s:", students->info->id, students->info->name);
        if (students->next != NULL)
            printf(", ");
        students = students->next;
    }
    printf("\n");
}

void free_all(slist* sl, clist* cl)
{
    free_students(sl, 1);
    free_courses(cl, 1);
}

void free_courses(clist* cl, int all_info)
{
    if (cl != NULL)
    {
        // Recursive call
        free_courses(cl->next, all_info);

        // Freeing itself
        if (all_info)
        {
            free_students(cl->info->students, 0);
            release_course(cl);
        }
        else
        {
            free(cl);
        }
    }
}

void free_students(slist* sl, int all_info)
{
    if (sl != NULL)
    {
        // Recursive call
        free_students(sl->next, all_info);

        // Freeing itself
        if (all_info)
        {
            free_courses(sl->info->courses, 0);
            release_student(sl);
        }
        else
        {
            free(sl);
        }
    }
}

void release_course(clist* cl)
{
    if (cl->info != NULL)
    {
        free(cl->info->title);
        free(cl->info);
    }
    free(cl);
}

void release_student(slist* sl)
{
    if (sl->info != NULL)
    {
        free(sl->info->name);
        free(sl->info);
    }
    free(sl);
}