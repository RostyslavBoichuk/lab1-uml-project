/**
 * @file fixed_matrix.cpp
 * @brief Object-oriented matrix calculator with multiple multiplication strategies.
 *
 * This is the improved architectural version of the matrix application.
 *
 * Features:
 *  - Generic Matrix class (template-based)
 *  - Encapsulated matrix operations (add, subtract, scalar multiply)
 *  - Strategy pattern for multiplication (Classical / Strassen)
 *  - MVC-like separation (Menu = View, Controller = Application logic)
 *  - Smart pointer usage for polymorphism
 *
 * The system is designed for:
 *  - Unit testing
 *  - Extensibility
 *  - Algorithm comparison
 *  - Performance evaluation
 *
 * @author Your Name
 * @version Final Lab Version
 */

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// Matrix

/**
 * @class Matrix
 * @brief Generic matrix container with basic operations.
 *
 * @tparam T Numeric type used for matrix elements.
 *
 * Responsibilities:
 *  - Store matrix data
 *  - Provide element access
 *  - Provide transpose operation
 *
 * Follows encapsulation principle — internal storage is hidden.
 */
template<typename T>
class Matrix {
private:
    vector<vector<T>> data;
public:
    /**
     * @brief Constructor
     * @param r Number of rows
     * @param c Number of columns
     */
    Matrix(int r = 0, int c = 0) : data(r, vector<T>(c, 0)) {}
    
    /** @return Number of rows */
    int rows() const { return data.size(); }

    /** @return Number of columns */
    int cols() const { return data.empty() ? 0 : data[0].size(); }

    /**
     * @brief Get element
     * @param r Row index
     * @param c Column index
     * @return Matrix element
     */
    T get(int r, int c) const { return data[r][c]; }

    /**
     * @brief Set element
     * @param r Row index
     * @param c Column index
     * @param val Value to assign
     */
    void set(int r, int c, T val) { data[r][c] = val; }
    
    /** Row access operator */
    vector<T>& operator[](int r) { return data[r]; }

    /** Const row access operator */
    const vector<T>& operator[](int r) const { return data[r]; }

    /**
     * @brief Print matrix to console.
     */
    void print() const {
        for (auto &row : data) {
            for (auto &val : row) cout << val << "\t";
            cout << endl;
        }
    }

    /**
     * @brief Compute transpose of matrix.
     * @return New transposed matrix.
     */
    Matrix<T> transpose() const {
        Matrix<T> R(cols(), rows());
        for (int i = 0; i < rows(); i++)
            for (int j = 0; j < cols(); j++)
                R.set(j, i, data[i][j]);
        return R;
    }
};

// Matrix Operations

/**
 * @class MatrixOperations
 * @brief Utility class containing basic matrix arithmetic operations.
 *
 * This class provides static methods for:
 *  - Addition
 *  - Subtraction
 *  - Scalar multiplication
 *
 * It acts as a service layer separating arithmetic logic
 * from UI and algorithm strategy.
 *
 * @tparam T Numeric type.
 */
template<typename T>
class MatrixOperations {
public:

    /**
     * @brief Adds two matrices element-wise.
     *
     * @param A First matrix.
     * @param B Second matrix.
     * @return Result matrix A + B.
     *
     * @pre Matrices must have equal dimensions.
     */
    static Matrix<T> add(const Matrix<T> &A, const Matrix<T> &B) {
        int r = A.rows(), c = A.cols();
        Matrix<T> R(r, c);
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                R.set(i, j, A.get(i,j) + B.get(i,j));
        return R;
    }

    /**
     * @brief Subtracts matrix B from matrix A.
     *
     * @param A First matrix.
     * @param B Second matrix.
     * @return Result matrix A - B.
     *
     * @pre Matrices must have equal dimensions.
     */
    static Matrix<T> subtract(const Matrix<T> &A, const Matrix<T> &B) {
        int r = A.rows(), c = A.cols();
        Matrix<T> R(r, c);
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                R.set(i, j, A.get(i,j) - B.get(i,j));
        return R;
    }

    /**
     * @brief Multiplies matrix by scalar.
     *
     * @param A Input matrix.
     * @param s Scalar value.
     * @return Scaled matrix.
     */
    static Matrix<T> scalarMultiply(const Matrix<T> &A, T s) {
        int r = A.rows(), c = A.cols();
        Matrix<T> R(r, c);
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                R.set(i, j, A.get(i,j) * s);
        return R;
    }
};

// Multiplier Interface

/**
 * @class IMatrixMultiplier
 * @brief Strategy interface for multiplication algorithms.
 *
 * Allows runtime switching between:
 *  - Classical algorithm
 *  - Strassen algorithm
 */
template<typename T>
class IMatrixMultiplier {
public:

    virtual Matrix<T> multiply(const Matrix<T> &A, const Matrix<T> &B) const = 0;
    virtual ~IMatrixMultiplier() = default;
};

// Classical Multiplier

/**
 * @class ClassicalMultiplier
 * @brief Standard O(n^3) multiplication.
 *
 * Used for small matrices.
 */
template<typename T>
class ClassicalMultiplier : public IMatrixMultiplier<T> {
public:
    Matrix<T> multiply(const Matrix<T> &A, const Matrix<T> &B) const override {
        int r = A.rows(), c = B.cols(), kMax = A.cols();
        Matrix<T> R(r, c);
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                T sum = 0;
                for (int k = 0; k < kMax; k++)
                    sum += A.get(i,k) * B.get(k,j);
                R.set(i, j, sum);
            }
        return R;
    }
};

// Strassen Multiplier

/**
 * @class StrassenMultiplier
 * @brief Recursive optimized multiplication.
 *
 * Uses divide-and-conquer strategy.
 * Switches to classical algorithm below threshold.
 */
template<typename T>
class StrassenMultiplier : public IMatrixMultiplier<T> {
private:
    int threshold = 64;
public:
    Matrix<T> multiply(const Matrix<T> &A, const Matrix<T> &B) const override {
        int n = A.rows() > A.cols() ? (A.rows() > B.cols() ? A.rows() : B.cols()) 
                             : (A.cols() > B.cols() ? A.cols() : B.cols());
        if (n <= threshold) return ClassicalMultiplier<T>().multiply(A,B);
        return strassen(A,B);
    }

private:
    Matrix<T> strassen(const Matrix<T> &A, const Matrix<T> &B) const {
        int n = A.rows() > A.cols() ? (A.rows() > B.cols() ? A.rows() : B.cols()) 
                             : (A.cols() > B.cols() ? A.cols() : B.cols());
        int m = 1; while(m<n) m<<=1;

        Matrix<T> AA(m,m), BB(m,m);
        for(int i=0;i<A.rows();i++)
            for(int j=0;j<A.cols();j++) AA.set(i,j,A.get(i,j));
        for(int i=0;i<B.rows();i++)
            for(int j=0;j<B.cols();j++) BB.set(i,j,B.get(i,j));

        Matrix<T> R = strassenRec(AA, BB);

        Matrix<T> res(A.rows(), B.cols());
        for(int i=0;i<A.rows();i++)
            for(int j=0;j<B.cols();j++)
                res.set(i,j,R.get(i,j));
        return res;
    }

    Matrix<T> strassenRec(const Matrix<T> &A,const Matrix<T> &B) const {
        int n = A.rows();
        if(n <= threshold) return ClassicalMultiplier<T>().multiply(A,B);

        int k = n/2;
        auto sub = [&](const Matrix<T>& M,int r,int c,int sz){
            Matrix<T> R(sz,sz);
            for(int i=0;i<sz;i++)
                for(int j=0;j<sz;j++)
                    R.set(i,j,M.get(r+i,c+j));
            return R;
        };

        Matrix<T> A11=sub(A,0,0,k), A12=sub(A,0,k,k),
                   A21=sub(A,k,0,k), A22=sub(A,k,k,k);
        Matrix<T> B11=sub(B,0,0,k), B12=sub(B,0,k,k),
                   B21=sub(B,k,0,k), B22=sub(B,k,k,k);

        Matrix<T> M1 = strassenRec(MatrixOperations<T>::add(A11,A22),
                                   MatrixOperations<T>::add(B11,B22));
        Matrix<T> M2 = strassenRec(MatrixOperations<T>::add(A21,A22), B11);
        Matrix<T> M3 = strassenRec(A11, MatrixOperations<T>::subtract(B12,B22));
        Matrix<T> M4 = strassenRec(A22, MatrixOperations<T>::subtract(B21,B11));
        Matrix<T> M5 = strassenRec(MatrixOperations<T>::add(A11,A12), B22);
        Matrix<T> M6 = strassenRec(MatrixOperations<T>::subtract(A21,A11),
                                   MatrixOperations<T>::add(B11,B12));
        Matrix<T> M7 = strassenRec(MatrixOperations<T>::subtract(A12,A22),
                                   MatrixOperations<T>::add(B21,B22));

        Matrix<T> C(n,n);
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++){
                C.set(i,j,M1.get(i,j)+M4.get(i,j)-M5.get(i,j)+M7.get(i,j));
                C.set(i,j+k,M3.get(i,j)+M5.get(i,j));
                C.set(i+k,j,M2.get(i,j)+M4.get(i,j));
                C.set(i+k,j+k,M1.get(i,j)+M3.get(i,j)-M2.get(i,j)+M6.get(i,j));
            }
        return C;
    }
};

// Menu (View)

/**
 * @class Menu
 * @brief Console user interface.
 *
 * Acts as View in MVC architecture.
 */
class Menu {
public:

    /** Show menu */
    int showMenu() {

        int choice;
        while (true) {
            cout << "\nChoose Operation:\n1. Addition\n2. Subtraction\n3. Scalar Multiply\n4. Multiply\n5. Transpose\nEnter choice: ";
    
            cin >> choice;
    
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter a number 1-5.\n";
                continue;
            }
    
            if (choice >= 1 && choice <= 5) break;
    
            cout << "Invalid choice. Please enter 1-5.\n";
        }
        return choice;
    }

    void printMessage(const string &msg) { cout << msg << endl; }
    void printMatrix(const Matrix<long long> &M) { M.print(); }
    void printError(const string &msg) { cerr << "Error: " << msg << endl; }
};

// Controller

/**
 * @class Controller
 * @brief Application core.
 *
 * Connects View and Business Logic.
 */
class Controller {
private:
    Menu menu;
public:

    /** Run application */
    void run() {
        
        char again='y';
        while(again=='y'||again=='Y') {
            int choice = menu.showMenu();
            handleChoice(choice);
            cout << "\nTry again? (Y/N): "; cin >> again;
        }
    }

private:
    void handleChoice(int choice) {
        Matrix<long long> A, B, C;
        long long scalar;
        unique_ptr<IMatrixMultiplier<long long>> multiplier;

        switch(choice) {
            case 1: case 2:
                A = inputMatrix("A"); B = inputMatrix("B");
                if(A.rows()!=B.rows()||A.cols()!=B.cols()) {
                    menu.printError("Matrices must have same size"); return;
                }
                C = (choice==1) ? MatrixOperations<long long>::add(A,B)
                                : MatrixOperations<long long>::subtract(A,B);
                menu.printMatrix(C); break;

            case 3:
                A = inputMatrix("A"); 
                cout << "Enter scalar: "; cin >> scalar;
                C = MatrixOperations<long long>::scalarMultiply(A, scalar);
                menu.printMatrix(C); break;

            case 4:
                A = inputMatrix("A"); B = inputMatrix("B");
                if(A.cols() != B.rows()) { menu.printError("Invalid sizes"); return; }
                multiplier = (A.rows() < 128) ? 
                    unique_ptr<IMatrixMultiplier<long long>>(new ClassicalMultiplier<long long>())
                    : unique_ptr<IMatrixMultiplier<long long>>(new StrassenMultiplier<long long>());
                C = multiplier->multiply(A,B);
                menu.printMatrix(C); break;

            case 5:
                A = inputMatrix("A"); 
                C = A.transpose(); 
                menu.printMatrix(C); break;

            default: menu.printError("Invalid choice");
        }
    }

    Matrix<long long> inputMatrix(const string &name) {
        int r,c;
        cout << "Enter size of " << name << " (rows cols): "; cin >> r >> c;
        Matrix<long long> M(r,c);
        cout << "Enter elements:\n";
        for(int i=0;i<r;i++)
            for(int j=0;j<c;j++) cin >> M[i][j];
        return M;
    }
};

#ifndef TESTING
int main() {
    Controller app;
    app.run();
    return 0;
}
#endif