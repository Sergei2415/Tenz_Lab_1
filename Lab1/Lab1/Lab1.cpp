#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

vector<int> result_path_tree_left;
vector<int> result_path_tree_right;
vector<int> tree_max;
//vector<int> tree_buf;
struct Node {
    int data;
    Node* left;
    Node* right;
};

// Переменные управления
string name_file = "tree2.txt";
Node* newRoot;
int global_maximum_path = 1;
int local_maximum_path = 1;


Node* createNode(int data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}
// Создать тестовый файл
void create_test_file() {
    ofstream file("data2.txt");
    if (file.is_open()) { //открываем файл
        for (int i = 0; i < 2097151; i++) {
            file << "1" << endl;
        }
        for (int i = 0; i < 2097151; i++) {
            file << "#" << endl;
        }
        file << "#";
        file.close(); // Закрываем файл
    }
    else {
        cout << "Unable to create file\n";
    }

    return;
}
// Сохранить дерево
void saveTree(Node* root, std::ofstream& file) {
    if (root == nullptr) {
        return;
    }

    std::queue<Node*> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        Node* current = nodes.front();
        nodes.pop();

        if (current == nullptr) {
            file << "#" << std::endl;
            continue;
        }

        file << current->data << std::endl;

        if (current->left != nullptr) {
            nodes.push(current->left);
        }
        else {
            nodes.push(nullptr);
        }

        if (current->right != nullptr) {
            nodes.push(current->right);
        }
        else {
            nodes.push(nullptr);
        }
    }
}
// Забилдить дерево в структуру
Node* buildTree(std::queue<std::string>& nodes) {
    std::string value = nodes.front();
    nodes.pop();

    if (value == "#") {
        return nullptr;
    }

    Node* newNode = createNode(std::stoi(value));

    std::queue<Node*> levelNodes;
    levelNodes.push(newNode);

    while (!levelNodes.empty()) {
        Node* current = levelNodes.front();
        levelNodes.pop();

        std::string leftValue = nodes.front();
        nodes.pop();

        if (leftValue != "#") {
            current->left = createNode(std::stoi(leftValue));
            levelNodes.push(current->left);
        }

        std::string rightValue = nodes.front();
        nodes.pop();

        if (rightValue != "#") {
            current->right = createNode(std::stoi(rightValue));
            levelNodes.push(current->right);
        }
    }

    return newNode;
}
// Вывести дерево
void printTree(Node* root, int space) {
    if (root == nullptr) {
        return;
    }

    space += 10;

    printTree(root->right, space);

    std::cout << std::endl;
    for (int i = 10; i < space; i++) {
        std::cout << " ";
    }
    std::cout << root->data << std::endl;

    printTree(root->left, space);
}

// Импорт данных
void importing_data() {
    // Read tree from file
    std::ifstream inputFile(name_file);
    std::string line;
    std::queue<std::string> nodes;

    while (std::getline(inputFile, line)) {
        nodes.push(line);
    }
    newRoot = buildTree(nodes);
}



// Поиск по концам
void finding_path_without_branching_in_the_ends(Node* newRootTest, vector<int> tree_buf = {}) {
    //cout << "Элемент - " << newRootTest->data << endl;

    if (newRootTest->left != nullptr && newRootTest->right != nullptr) {
        tree_buf.clear();
        local_maximum_path = 1;
        finding_path_without_branching_in_the_ends(newRootTest->left, tree_buf);
        local_maximum_path = 1;
        finding_path_without_branching_in_the_ends(newRootTest->right, tree_buf);
    }
    else if (newRootTest->left != nullptr && newRootTest->right == nullptr) {
        local_maximum_path += 1;
        tree_buf.push_back(newRootTest->data);
        finding_path_without_branching_in_the_ends(newRootTest->left, tree_buf);
    }
    else if (newRootTest->right != nullptr && newRootTest->left == nullptr) {
        local_maximum_path += 1;
        tree_buf.push_back(newRootTest->data);
        finding_path_without_branching_in_the_ends(newRootTest->right, tree_buf);
    }
    else {
       // cout << "Ответвление - ";
        tree_buf.push_back(newRootTest->data);
        for (int i : tree_buf) {
            //cout << i << " ";
        }
        if (tree_max.size() < tree_buf.size()) {
            tree_max.clear();
            for (int i : tree_buf) {
                //cout << i << " ";
                tree_max.push_back(i);
            }
        }
        cout << endl;
       // cout << "global_maximum_path - " << global_maximum_path <<endl;
        if (global_maximum_path < tree_buf.size() + 1)
            global_maximum_path = tree_buf.size() + 1;
        local_maximum_path = 1;
    }

}

// Нати пути от корня
void finding_path_without_branching() {



    int maximum_path_without_branches = 0;
    int local_maximum_path_without_branches = 1;
    int flag = 0;
    Node* newRootTestLeft = newRoot->left;
    Node* newRootTestRight = newRoot->right;
    result_path_tree_left.push_back(newRoot->left->data);
    result_path_tree_right.push_back(newRoot->data);
    result_path_tree_right.push_back(newRoot->right->data);
    if (newRoot->left != nullptr && newRoot->right != nullptr) {

        while (true) {
            if (newRootTestLeft->left != nullptr && newRootTestLeft->right == nullptr) {
                local_maximum_path_without_branches += 1;
                newRootTestLeft = newRootTestLeft->left;

            }
            else if (newRootTestLeft->right != nullptr && newRootTestLeft->left == nullptr) {
                local_maximum_path_without_branches += 1;
                newRootTestLeft = newRootTestLeft->right;

            }
            else if (newRootTestLeft->left != nullptr && newRootTestLeft->right != nullptr) {
                local_maximum_path_without_branches += 1; //проверить
                flag += 1;
                finding_path_without_branching_in_the_ends(newRootTestLeft);
                break;
            }
            else {
                break;
            }
            result_path_tree_left.push_back(newRootTestLeft->data);
        }

        while (true) {
            if (newRootTestRight->left != nullptr && newRootTestRight->right == nullptr) {
                local_maximum_path_without_branches += 1;
                newRootTestRight = newRootTestRight->left;

            }
            else if (newRootTestRight->right != nullptr && newRootTestRight->left == nullptr) {
                local_maximum_path_without_branches += 1;
                newRootTestRight = newRootTestRight->right;

            }
            else if (newRootTestRight->right != nullptr && newRootTestRight->left != nullptr) {
                local_maximum_path_without_branches += 1; //проверить
                flag += 1;

                break;
            }
            else {
                break;
            }
            result_path_tree_right.push_back(newRootTestRight->data);
        }

        //cout << "Правый верктор - ";
        for (int i : result_path_tree_right) {
           // cout << i << " ";
        }
        cout << endl;
        reverse(result_path_tree_left.begin(), result_path_tree_left.end());
        //cout << "Левый верктор - ";
        for (int i : result_path_tree_left) {
            //cout << i << " ";
        }
        cout << endl;
    }
    if (flag != 2)
        maximum_path_without_branches = local_maximum_path_without_branches;


    if (maximum_path_without_branches < global_maximum_path) {
        cout << "\nИтоговый ответ - " << global_maximum_path << "\n";
        for (int i : tree_max) {
            cout << i << " ";
        }
    }
    else {
        cout << "\nИтоговый ответ - " << maximum_path_without_branches + 1 << "\n";
        for (int i : result_path_tree_left) {
            cout << i << " ";
        }
        for (int i : result_path_tree_right) {
            cout << i << " ";
        }
    }
}

int main() {



    setlocale(LC_ALL, "Russian");

    int choice = 4;
    while (true) {
        cout << "\nДоступные опции:\n";
        cout << "1. Создание тестового файла\n";
        cout << "2. Импорт данных из файла\n";
        cout << "3. Нахождение в дереве самого длинного пути без ветвлений\n";
        cout << "4. Вывести дерево\n";
        cout << "5. Выход\n";
        cout << "Введите число(номер опции): ";
        cin >> choice;

        switch (choice) {
        case 1:
            create_test_file();
            break;
        case 2:
            importing_data();
            break;
        case 3:
            finding_path_without_branching();
            break;
        case 4:
            printTree(newRoot, 0);
            break;
        case 5:
            cout << "Выход...\n";
            return 0;
        default:
            cout << "Некорректный ввод опции! Повторите попытку.\n";
            break;
        
    }}

    return 0;

}





