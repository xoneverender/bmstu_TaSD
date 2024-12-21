#include "actions_processing.h"

err_code_e process_action(action_e action, tree_node_t **root)
{
    err_code_e rc = ERR_SUCCESS;

    puts("");
    horizontal_rule();

    switch (action)
    {
        case EXIT:
            puts("\nВыход из программы...\n");
            free_tree(root);
            exit(0);
            break;
        case ADD_STUDENT:
            printf("\nДОБАВЛЕНИЕ ИНФОРМАЦИИ О СТУДЕНТЕ\n");

            rc = operate_adding_student(root);

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nДобавление информации выполнено успешно" RESET);
            break;
        case REMOVE_STUDENT:
            puts("\nУДАЛЕНИЕ СТУДЕНТА\n");

            rc = operate_removing_student(root);

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nУдаление информации о студенте выполнено успешно." RESET);
            break;
        case REMOVE_STUDENTS_WITH_BAD_MARKS:
            puts("\nУДАЛЕНИЕ ВСЕХ СТУДЕНТОВ С БАЛЛОМ НИЖЕ ИЛИ РАВНЫМ 2.0\n");

            rc = operate_removing_low_mark_nodes(root);

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nУдаление информации о студентах выполнено успешно." RESET);
            break;
        case FIND_STUDENT:
            puts("\nПОИСК СТУДЕНТА\n");

            rc = operate_finding_student_info(root);

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nПоиск информации о студенте выполнено успешно." RESET);
            break;
        case UPLOAD_TREE_DATA:
            puts("\nЗАГРУЗКА ДЕРЕВА ИЗ ФАЙЛА\n");

            rc = operate_uploading_data_from_file(root);

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nЗагрузка прошла успешно." RESET);
            break;
        case SHOW_TREE:
            puts("\nВЫВОД ДЕРЕВА\n");

            rc = operate_printing_tree(*root);

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nДемонстрация дерева выполнена успешно." RESET);
            break;
        case COMPARE_ALGS:
            puts("\nСРАВНЕНИЕ ВРЕМЕНИ УДАЛЕНИЯ В ДЕРЕВЕ, ПОСТРОЕННОМ ПО ФАМИЛИИ, С ДЕРЕВОМ, ПОСТРОЕННЫМ ПО БАЛЛУ\n");

            operate_comparizon();

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nСравнение двух реализаций прошло успешно." RESET);
            break;
    }
    return rc;
}

err_code_e operate_adding_student(tree_node_t **root)
{
    student_t *new_student;
    err_code_e rc = ERR_SUCCESS;
    char *prompt, *error_message;
    char *surname;
    double mark;

    prompt = "\nВведите фамилию студента:";
    error_message = "Ошибка чтения строки. Повторите ввод.";
    if ((rc = get_string_from_stdin(prompt, &surname, error_message)) != ERR_SUCCESS)
        return rc;

    prompt = "Введите средний балл студента: ";
    error_message = "Введенное строка должна быть вещественным неотрицательным числом. Повторите ввод.";
    if ((rc = get_double_from_stdin(prompt, &mark, 0, DBL_MAX, error_message)) != ERR_SUCCESS)
        return rc;

    new_student = create_student(surname, mark);
    if (new_student == NULL)
        return ERR_ALLOCATING_MEMORY;

    rc = insert(root, new_student, strings_compare);

    return rc;
}

err_code_e operate_removing_student(tree_node_t **root)
{
    tree_node_t *student_to_delete;
    err_code_e rc;
    char *prompt, *error_message;
    char *surname;

    if (*root == NULL)
        return ERR_EMPTY_TREE;


    prompt = "Введите фамилию студента, информацию о котором хотите удалить:";
    error_message = "Ошибка чтения строки. Повторите ввод.";
    if ((rc = get_string_from_stdin(prompt, &surname, error_message)) != ERR_SUCCESS)
        return rc;

    student_to_delete = find_by_surname(*root, surname);
    if (student_to_delete == NULL)
        return ERR_NOT_FOUND;

    remove_node(root, student_to_delete);

    free(surname);

    return ERR_SUCCESS;
}

err_code_e operate_removing_low_mark_nodes(tree_node_t **root)
{
    if (*root == NULL)
        return ERR_EMPTY_TREE;
    
    operate_removing_low_mark_nodes(&(*root)->left);
    operate_removing_low_mark_nodes(&(*root)->right);  
    if ((*root)->student->average_mark <= 2.0)
        remove_node(root, *root);

    return ERR_SUCCESS;
}

err_code_e operate_finding_student_info(tree_node_t **root)
{
    tree_node_t *student;
    err_code_e rc;
    char *prompt, *error_message;
    char *surname;

    if (*root == NULL)
        return ERR_EMPTY_TREE;

    prompt = "Введите фамилию студента, баллы которого вы хотите найти:";
    error_message = "Ошибка чтения строки. Повторите ввод.";
    if ((rc = get_string_from_stdin(prompt, &surname, error_message)) != ERR_SUCCESS)
        return rc;

    student = find_by_surname(*root, surname);
    free(surname);

    if (student == NULL)
        return ERR_NOT_FOUND;

    puts("");
    print_node(student);

    return ERR_SUCCESS;
}

err_code_e operate_uploading_data_from_file(tree_node_t **root)
{
    err_code_e rc;
    char *prompt, *error_message;
    char *filename;

    if (*root != NULL)
    {
        puts(YELLOW "Предупреждение: Данные, которые уже хранятся в дереве будут удалены.\n" RESET);
        free_tree(root);
    }

    prompt = "Введите название файла, данные из которого вы хотите считать:";
    error_message = "Ошибка чтения строки. Повторите ввод.";
    if ((rc = get_string_from_stdin(prompt, &filename, error_message)) != ERR_SUCCESS)
        return rc;

    if ((rc = read_data_from_file(filename, root)) != ERR_SUCCESS)
    {
        free(filename);
        return rc;
    }

    free(filename);

    return ERR_SUCCESS;
}

err_code_e operate_printing_tree(tree_node_t *root)
{
    err_code_e rc;
    char *prompt, *error_message;
    long choice;
    tree_output_e format;

    if (root == NULL)
        return ERR_EMPTY_TREE;

    prompt = "Выберите формат, в котором дерево будет выведено:\n1. Инфиксный\n2. Префиксный\n3. Постфиксный\n4. Графическое представление";
    error_message = "Выбранный пункт меню должен быть целым числом от 1 до 4. Повторите ввод.";
    if ((rc = get_int_from_stdin(prompt, &choice, 1, 4, error_message)) != ERR_SUCCESS)
        return rc;
    format = (tree_output_e)choice;

    puts("");

    if (format == INORDER)
        infix_traversal(root);
    else if (format == PREORDER)
        prefix_traversal(root);
    else if (format == POSTORDER)
        postfix_traversal(root);
    else if (format == GRAPHICAL)
        rc = export_tree_to_dot(root, "graph.gv", "graph.png");

    return rc;    
}

// Функция для генерации случайной строки (фамилии)
char *generate_random_surname(size_t length) 
{
    char *surname = malloc(length + 1);
    for (size_t i = 0; i < length; i++) 
        surname[i] = 'A' + rand() % 26; // Генерация случайной буквы

    surname[length] = '\0';
    return surname;
}

// Функция для генерации массива студентов
student_t **generate_students(size_t count) 
{
    student_t **students = malloc(count * sizeof(student_t *));
    char *surname;
    double mark;
    size_t letters_count = 0;
    
    srand(time(NULL));

    for (size_t i = 0; i < count; i++) 
    {
        letters_count = rand() % (10 - 5 + 1) + 5;
        surname = generate_random_surname(letters_count); // Генерация случайной фамилии длиной 5 символов
        mark = (rand() / (double)RAND_MAX) * 5.0; // Случайный балл от 0.0 до 5.0
        students[i] = create_student(surname, mark);
    }
    return students;
}

student_t **deep_copy_array(size_t count, student_t **src)
{
    student_t **dst = malloc(count * sizeof(student_t *));

    for (size_t i = 0; i < count; i++)
    {
        dst[i] = malloc(sizeof(student_t));
        dst[i]->average_mark = src[i]->average_mark;
        dst[i]->surname = strdup(src[i]->surname);
    }

    return dst;
}

unsigned long long us_now(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);

    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
    // return (unsigned long long)value.tv_sec * 1000000000ULL + (unsigned long long)value.tv_usec * 1000ULL;
}

// void operate_comparizon(void)
// {
//     unsigned long long start, end;
//     double time_by_mark = 0, time_by_surname = 0, percent_diff = 0;
//     student_t **students_by_mark, **students_by_surname;
//     tree_node_t *tree_by_mark;
//     tree_node_t *tree_by_surname;

//     printf("    Количество элементов|    Время удаления по баллу, нс|    Время удаления по фамилии, нс|    Разница, %%\n");
//     puts("   -------------------------------------------------------------------------------------------------------");
//     for (size_t num_students = MIN_ELEMENTS; num_students <= MAX_ELEMENTS; num_students += STEP)
//     {
//         tree_by_mark = NULL;
//         tree_by_surname = NULL;
        
//         for (size_t i = 0; i < ITERATIONS; i++)
//         {   
//             // Генерация тестовых данных (массива студентов)
//             students_by_mark = generate_students(num_students);
//             students_by_surname = deep_copy_array(num_students, students_by_mark);

//             // Заполнение деревьев
//             for (size_t i = 0; i < num_students; i++) 
//             {
//                 (void)insert(&tree_by_mark, students_by_mark[i], double_compare);
//                 (void)insert(&tree_by_surname, students_by_surname[i], strings_compare);
//             }

//             start = us_now();
//             operate_removing_low_mark_nodes(&tree_by_mark);
//             end = us_now();

//             time_by_mark += end - start;

//             start = us_now();
//             operate_removing_low_mark_nodes(&tree_by_surname);
//             end = us_now();

//             time_by_surname += end - start;

//             free_tree(&tree_by_mark);
//             free_tree(&tree_by_surname);
//             // free(students);
//         }

//         time_by_mark /= (double)ITERATIONS;
//         time_by_surname /= (double)ITERATIONS;

//         if (time_by_mark != 0)
//             percent_diff = ((time_by_surname - time_by_mark) / time_by_mark) * 100.0;
//         else
//             percent_diff = 0; // В случае, если время по баллу равно 0, разница не рассчитывается

//         printf("%24zu|\t%24.2f|\t%26.2f|\t%9.2f\n", num_students, time_by_mark, time_by_surname, percent_diff);
//     }
// }

void rebuilding_tree_by_mark(tree_node_t *tree_by_surname, tree_node_t **tree_by_mark)
{
    student_t *new_student;

    if (tree_by_surname == NULL)
        return;

    rebuilding_tree_by_mark(tree_by_surname->left, tree_by_mark);

    new_student = malloc(sizeof(student_t));
    new_student->average_mark = tree_by_surname->student->average_mark;
    new_student->surname = strdup(tree_by_surname->student->surname);

    if (insert(tree_by_mark, new_student, double_compare))
        return;

    rebuilding_tree_by_mark(tree_by_surname->right, tree_by_mark);
}

void operate_comparizon(void)
{
    unsigned long long start, end;
    double time_by_mark = 0, time_by_surname = 0, rebuilding_time = 0;
    // double percent_diff = 0;
    student_t **students;
    tree_node_t *tree_by_mark;
    tree_node_t *tree_by_surname;

    // printf("    Количество элементов|    Время удаления по фамилии, мкс|    Время перестроения по баллу, мкс|    Время удаления по баллу, мкс|    Разница, %%\n");
    printf("    Количество элементов|    Время удаления по фамилии, мкс|    Время перестроения по баллу, мкс|    Время удаления по баллу, мкс\n");
    puts("   -------------------------------------------------------------------------------------------------------------------------------");
    for (size_t num_students = MIN_ELEMENTS; num_students <= MAX_ELEMENTS; num_students += STEP)
    {
        tree_by_mark = NULL;
        tree_by_surname = NULL;
        
        for (size_t i = 0; i < ITERATIONS; i++)
        {   
            // Генерация тестовых данных (массива студентов)
            students = generate_students(num_students);

            // Заполнение дерева по фамилии
            for (size_t i = 0; i < num_students; i++) 
                (void)insert(&tree_by_surname, students[i], strings_compare);

            // Перестроение дерева
            start = us_now();
            rebuilding_tree_by_mark(tree_by_surname, &tree_by_mark);
            end = us_now();
            rebuilding_time += end - start;

            // Удаление в дереве, построенном по фамилии
            start = us_now();
            operate_removing_low_mark_nodes(&tree_by_surname);
            end = us_now();
            time_by_surname += end - start;

            // Удаление в дереве, построенном по баллу
            start = us_now();
            operate_removing_low_mark_nodes(&tree_by_mark);
            end = us_now();
            time_by_mark += end - start;

            free_tree(&tree_by_mark);
            free_tree(&tree_by_surname);
            // free(students);
        }

        time_by_mark /= (double)ITERATIONS;
        time_by_surname /= (double)ITERATIONS;
        rebuilding_time /= (double)ITERATIONS;
        
        // if (time_by_mark != 0)
        //     percent_diff = ((time_by_surname - time_by_mark) / time_by_mark) * 100.0;
        // else
        //     percent_diff = 0; // В случае, если время по баллу равно 0, разница не рассчитывается

        // printf("%24zu|\t%27.2f|\t%25.2f|\t%25.2f|\t%9.2f\n", num_students, time_by_surname, rebuilding_time, time_by_mark, percent_diff);
        printf("%24zu|\t%27.2f|\t%32.2f|\t%25.2f\n", num_students, time_by_surname, rebuilding_time, time_by_mark);
    }
}
