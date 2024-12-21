#include "tree.h"

int strings_compare(student_t *student_1, student_t *student_2)
{
    return strcmp(student_1->surname, student_2->surname);
}

int double_compare(student_t *student_1, student_t *student_2)
{
    const double diff = student_1->average_mark - student_2->average_mark;

    if (fabs(diff) < 1e-6) 
        return 0;
    if (diff > 0) 
        return 1;

    return -1;
}

// Функция для создания нового узла
tree_node_t* create_node(student_t *student)
{
    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (!node) 
        return NULL;

    node->student = student;
    node->left = NULL;
    node->right = NULL;

    return node;
}

student_t* create_student(char *surname, double mark)
{
    student_t *student = malloc(sizeof(student_t));
    if (!student)
        return NULL;

    student->surname = surname;
    student->average_mark = mark;

    return student;
}

// Функция для вставки нового узла в дерево
err_code_e insert(tree_node_t **root, student_t *student, int (*comparator)(student_t *, student_t *))
{
    err_code_e rc;

    if (*root == NULL) 
    {
        *root = create_node(student);
        return ERR_SUCCESS;
    }

    if (comparator(student, (*root)->student) == 0)
        return ERR_IS_A_DUBLICATE;

    if (comparator(student, (*root)->student) < 0)
    {
        rc = insert(&(*root)->left, student, comparator);
        if (rc)
            return rc;
    }
    else if (comparator(student, (*root)->student) > 0)
    {
        rc = insert(&(*root)->right, student, comparator);
        if (rc)
            return rc;
    }

    return ERR_SUCCESS;
}

// Функция для поиска студента по фамилии
tree_node_t* find_by_surname(tree_node_t *root, char *surname)
{
    if (root == NULL || strcmp(root->student->surname, surname) == 0) 
        return root;

    if (strcmp(surname, root->student->surname) < 0)
        return find_by_surname(root->left, surname);
    else
        return find_by_surname(root->right, surname);
}

// Функция для поиска студента по фамилии
tree_node_t* find_by_mark(tree_node_t *root, double mark)
{
    if (root == NULL || root->student->average_mark == mark) 
        return root;

    if (root->student->average_mark < mark)
        return find_by_mark(root->left, mark);
    else
        return find_by_mark(root->right, mark);
}

tree_node_t* find_predecessor(tree_node_t *tree, tree_node_t *node)
{
    tree_node_t *found;

    if (tree == NULL)
        return NULL;

    if (tree->left == node || tree->right == node)
        return tree;

    if ((found = find_predecessor(tree->left, node)) != NULL)
        return found;
    else
        return find_predecessor(tree->right, node);
}

tree_node_t* get_max_node(tree_node_t *tree)
{
    if (!tree->right)
        return NULL;
    
    while (tree->right)
        tree = tree->right;

    return tree;
}

void remove_node(tree_node_t **tree, tree_node_t *node)
{
    tree_node_t *max_node, *predecessor, *temp;

    // Если удаляемый узел является корнем дерева
    if (*tree == node)
    {
        // Случай 1: узел является листом (нет потомков)
        if (node->left == NULL && node->right == NULL)
        {
            *tree = NULL;
            free(node);
            return;
        }

        // Случай 2: у узла есть только левый потомок
        if (node->left && node->right == NULL)
        {
            *tree = node->left;
            free(node);
            return;
        }

        // Случай 3: у узла есть только правый потомок
        if (node->right && node->left == NULL)
        {
            *tree = node->right; 
            free(node);
            return;
        }

        // Случай 4: у узла есть оба потомка
        // Ищем максимальный узел в левом поддереве
        max_node = get_max_node(node->left);

        // Если левое поддерево существует, но его корень не имеет правого потомка
        if (max_node == NULL)
        {
            max_node = node->left;
            *tree = max_node;
            max_node->right = node->right; 
            free(node);
            return;
        }

        // Находим предшественника (родителя max_node)
        temp = find_predecessor(*tree, max_node);

        *tree = max_node; // Заменяем удаляемый узел на max_node
        max_node->right = node->right; // Привязываем правое поддерево
        temp->right = max_node->left; // Перенаправляем левое поддерево max_node
        max_node->left = node->left; // Привязываем левое поддерево
        free(node); // Удаляем узел
        return;
    }

    // Если удаляемый узел не является корнем
    predecessor = find_predecessor(*tree, node);

    // Случай 1: узел является листом
    if (node->left == NULL && node->right == NULL)
    {
        if (predecessor->left == node) // Узел - левый потомок предшественника
            predecessor->left = NULL;
        else // Узел - правый потомок предшественника
            predecessor->right = NULL;
        
        free(node); // Освобождаем память
        return;
    }

    // Случай 2: у узла есть только левый потомок
    if (node->left && node->right == NULL)
    {
        if (predecessor->left == node)
            predecessor->left = node->left; // Привязываем левое поддерево к предшественнику
        else
            predecessor->right = node->left;

        free(node); // Удаляем узел
        return;
    }

    // Случай 3: у узла есть только правый потомок
    if (node->right && node->left == NULL)
    {
        if (predecessor->left == node)
            predecessor->left = node->right; // Привязываем правое поддерево к предшественнику
        else
            predecessor->right = node->right;

        free(node); // Удаляем узел
        return;
    }

    // Случай 4: у узла есть оба потомка
    max_node = get_max_node(node->left);

    if (max_node == NULL)
    {
        max_node = node->left;
        if (predecessor->left == node)
            predecessor->left = max_node;
        else
            predecessor->right = max_node;
        max_node->right = node->right; // Привязываем правое поддерево
        free(node);
        return;
    }

    // Находим предшественника max_node
    temp = find_predecessor(*tree, max_node);

    if (predecessor->left == node)
        predecessor->left = max_node; // Заменяем узел на max_node
    else
        predecessor->right = max_node;

    max_node->right = node->right; // Привязываем правое поддерево
    temp->right = max_node->left; // Перенаправляем левое поддерево max_node
    max_node->left = node->left; // Привязываем левое поддерево
    free(node); // Удаляем узел

    return;
}

// Функция для вывода информации о студенте
void print_node(tree_node_t *node)
{
    if (node == NULL || node->student == NULL || node->student->surname == NULL)
    {
        puts("NULL");
        return;
    }
    printf("Фамилия: %s, Средний балл: %.2f\n", node->student->surname, node->student->average_mark);
}

void remove_low_mark_nodes(tree_node_t **root)
{
    if (*root == NULL)
        return;

    remove_low_mark_nodes(&(*root)->left);
    remove_node(root, *root);
    remove_low_mark_nodes(&(*root)->right);   
}

// Инфиксиный обход
void infix_traversal(tree_node_t *root)
{
    if (root == NULL)
        return;

    infix_traversal(root->left);
    print_node(root);
    infix_traversal(root->right);
}   

// Префиксный обход
void prefix_traversal(tree_node_t *root)
{
    if (root == NULL) 
        return;

    print_node(root);
    prefix_traversal(root->left);
    prefix_traversal(root->right);
}

// Постфиксный обход
void postfix_traversal(tree_node_t *root)
{
    if (root == NULL) return;

    postfix_traversal(root->left);
    postfix_traversal(root->right);
    print_node(root);
}

void free_student(student_t **student)
{
    if (student == NULL || *student == NULL)
        return;

    free((*student)->surname);
    free(*student);
    *student = NULL;
}

// Функция для освобождения памяти, занятой деревом
void free_tree(tree_node_t **root)
{
    if (root == NULL || *root == NULL) 
        return;

    free_tree(&(*root)->left);
    free_tree(&(*root)->right);
    free_student(&(*root)->student);
    free(*root);
    *root = NULL;
}

void export_node(tree_node_t *node, FILE *f) 
{
    if (node == NULL) 
        return;

    fprintf(f, "    \"%s\" [label=\"{ %s | %.2f }\"];\n", node->student->surname, node->student->surname, node->student->average_mark);

    if (node->left) 
    {
        fprintf(f, "    \"%s\" -> \"%s\";\n", node->student->surname, node->left->student->surname);
        export_node(node->left, f);
    }

    if (node->right) 
    {
        fprintf(f,"    \"%s\" -> \"%s\";\n", node->student->surname, node->right->student->surname);
        export_node(node->right, f);
    }
}

err_code_e export_tree_to_dot(tree_node_t *root, const char *dot_filename, const char *output_filename)
{
    char command[256];
    FILE *file = fopen(dot_filename, "w");
    if (!file) 
        return ERR_OPENING_FILE;

    fprintf(file, "digraph BST {\n");
    fprintf(file, "    node [shape=record];\n");

    export_node(root, file);

    fprintf(file, "}\n");
    fclose(file);

    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", dot_filename, output_filename);
    
    if (system(command) != 0)
        return ERR_GRAPHVIZ_CONVERSION;
    else
        printf("Дерево успешно экспортировано в изображение \"%s\".\n", output_filename);

    return ERR_SUCCESS;
}
