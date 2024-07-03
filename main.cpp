#include <iostream>
#include <vector>

class Node {
public:
    int row;
    int col;
    int value;
    Node* right;
    Node* down;

    Node(int r, int c, int val) : row(r), col(c), value(val), right(nullptr), down(nullptr) {}
};

class HeaderNode {
public:
    int index;
    Node* next;
    HeaderNode* nextHeader;

    HeaderNode(int idx) : index(idx), next(nullptr), nextHeader(nullptr) {}
};

class SparseMatrix {
private:
    HeaderNode* rowHeaders;
    HeaderNode* colHeaders;
    int rows;
    int cols;

public:
    SparseMatrix(int r, int c) : rows(r), cols(c) {
        rowHeaders = new HeaderNode(-1);
        colHeaders = new HeaderNode(-1);

        HeaderNode* currentRowHeader = rowHeaders;
        HeaderNode* currentColHeader = colHeaders;

        for (int i = 0; i < rows; ++i) {
            currentRowHeader->nextHeader = new HeaderNode(i);
            currentRowHeader = currentRowHeader->nextHeader;
        }

        for (int j = 0; j < cols; ++j) {
            currentColHeader->nextHeader = new HeaderNode(j);
            currentColHeader = currentColHeader->nextHeader;
        }
    }

    void insert(int r, int c, int val) {
        if (r >= rows || c >= cols) {
            std::cerr << "Index out of bounds" << std::endl;
            return;
        }

        Node* newNode = new Node(r, c, val);

        // Insert into row list
        HeaderNode* rowHeader = rowHeaders->nextHeader;
        while (rowHeader && rowHeader->index != r) {
            rowHeader = rowHeader->nextHeader;
        }
        if (rowHeader) {
            Node* current = rowHeader->next;
            Node* prev = nullptr;
            while (current && current->col < c) {
                prev = current;
                current = current->right;
            }
            newNode->right = current;
            if (prev) {
                prev->right = newNode;
            } else {
                rowHeader->next = newNode;
            }
        }

        // Insert into column list
        HeaderNode* colHeader = colHeaders->nextHeader;
        while (colHeader && colHeader->index != c) {
            colHeader = colHeader->nextHeader;
        }
        if (colHeader) {
            Node* current = colHeader->next;
            Node* prev = nullptr;
            while (current && current->row < r) {
                prev = current;
                current = current->down;
            }
            newNode->down = current;
            if (prev) {
                prev->down = newNode;
            } else {
                colHeader->next = newNode;
            }
        }
    }

    void printMatrix() const {
        for (HeaderNode* rowHeader = rowHeaders->nextHeader; rowHeader != nullptr; rowHeader = rowHeader->nextHeader) {
            Node* current = rowHeader->next;
            int colIndex = 0;
            while (current != nullptr) {
                while (colIndex < current->col) {
                    std::cout << "0 ";
                    colIndex++;
                }
                std::cout << current->value << " ";
                current = current->right;
                colIndex++;
            }
            while (colIndex < cols) {
                std::cout << "0 ";
                colIndex++;
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    SparseMatrix sm(4, 5);
    sm.insert(0, 1, 3);
    sm.insert(1, 2, 5);
    sm.insert(2, 0, 2);
    sm.insert(3, 4, 4);

    sm.printMatrix();

    return 0;
}
