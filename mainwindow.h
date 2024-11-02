#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QFormLayout>
#include <QDateEdit>  // Thêm QDateEdit để nhập ngày
#include <QDate>
#include <QMainWindow>
#include <QTableWidget>
#include <QStringList>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>

class MainWindow;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Định nghĩa cấu trúc Student
struct Student {
    QString id;
    QString name;
    QString birthDate;
    QString gender;
    QString className;
    QString major;
    QString khoa;
    QString soKhoaHoc;
    QString sdt;
    QString diachi;
};

// Định nghĩa lớp StudentList với danh sách liên kết
class StudentList {
public:

    struct Node {
        Student student;
        Node* next;
        Node(const Student& s) : student(s), next(nullptr) {}
    };

    StudentList() : head(nullptr) {}
    ~StudentList() { clearList(); }

    void append(const Student& student);
    void inputStudentListFromCSV(const QString& filename);
    void outputStudentListToCSV(const QString& filename);
    void printStudentList(QTableWidget* tableWidget, MainWindow* mainWindow);
    bool deleteStudent(const QString& id);
    void sortByID();
    void clearList();
private:
    Node* head;
    std::vector<Student> students;
};

class AddStudentDialog : public QDialog {
    //Q_OBJECT
public:
    explicit AddStudentDialog(QWidget* parent = nullptr) : QDialog(parent) {
        setWindowTitle("Thêm sinh viên");

        // Tạo các trường nhập liệu
        idInput = new QLineEdit(this);
        nameInput = new QLineEdit(this);
        birthDateInput = new QDateEdit(this);  // Dùng QDateEdit để nhập ngày
        birthDateInput->setDisplayFormat("dd/MM/yyyy"); // Định dạng ngày
        birthDateInput->setCalendarPopup(true);  // Hiển thị popup lịch
        genderInput = new QLineEdit(this);
        classNameInput = new QLineEdit(this);
        majorInput = new QLineEdit(this);
        khoaInput = new QLineEdit(this);
        soKhoaHocInput = new QLineEdit(this);
        sodienthoaiInput = new QLineEdit(this);
        diachiInput = new QLineEdit(this);

        // Đặt chiều rộng tối thiểu cho các ô input
        idInput->setMinimumWidth(300);
        nameInput->setMinimumWidth(300);
        birthDateInput->setMinimumWidth(300);
        genderInput->setMinimumWidth(300);
        classNameInput->setMinimumWidth(300);
        majorInput->setMinimumWidth(300);
        khoaInput->setMinimumWidth(300);
        soKhoaHocInput->setMinimumWidth(300);
        sodienthoaiInput->setMinimumWidth(300);
        diachiInput->setMinimumWidth(300);

        // Đặt chiều cao tối thiểu cho các ô input
        idInput->setMinimumHeight(35);
        nameInput->setMinimumHeight(35);
        birthDateInput->setMinimumHeight(35);
        genderInput->setMinimumHeight(35);
        classNameInput->setMinimumHeight(35);
        majorInput->setMinimumHeight(35);
        khoaInput->setMinimumHeight(35);
        soKhoaHocInput->setMinimumHeight(35);
        sodienthoaiInput->setMinimumHeight(35);
        diachiInput->setMinimumHeight(35);


        // Tạo layout và thêm các trường vào
        QFormLayout* formLayout = new QFormLayout;
        formLayout->addRow("MSSV:", idInput);
        formLayout->addRow("Họ và Tên:", nameInput);
        formLayout->addRow("Ngày sinh:", birthDateInput);  // Thêm QDateEdit cho ngày sinh
        formLayout->addRow("Giới tính:", genderInput);
        formLayout->addRow("Lớp:", classNameInput);
        formLayout->addRow("Ngành:", majorInput);
        formLayout->addRow("Khoa:", khoaInput);
        formLayout->addRow("Khóa:", soKhoaHocInput);
        formLayout->addRow("Số điện thoại:", sodienthoaiInput);
        formLayout->addRow("Địa chỉ:", diachiInput);

        // Tạo nút OK và Cancel
        QPushButton* okButton = new QPushButton("OK", this);
        okButton->setMinimumHeight(35);
        okButton->setStyleSheet("background-color: blue; color: white; font-size: 14px; font-weight: bold;");
        QPushButton* cancelButton = new QPushButton("Hủy", this);
        cancelButton->setMinimumHeight(35);
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);

        // Kết nối nút OK và Cancel
        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        // Tạo layout chính
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addLayout(formLayout);
        mainLayout->addLayout(buttonLayout);

        setLayout(mainLayout);
    }

    // Hàm để lấy thông tin sinh viên từ dialog
    QString getId() const { return idInput->text(); }
    QString getName() const { return nameInput->text(); }
    QDate getBirthDate() const { return birthDateInput->date(); }  // Trả về QDate thay vì chuỗi
    QString getGender() const { return genderInput->text(); }
    QString getClassName() const { return classNameInput->text(); }
    QString getMajor() const { return majorInput->text(); }
    QString getKhoa() const { return khoaInput->text(); }
    QString getSoKhoaHoc() const { return soKhoaHocInput->text(); }
    QString getSoDienThoai() const { return sodienthoaiInput->text(); }
    QString getDiaChi() const { return diachiInput->text(); }

private:
    QLineEdit* idInput;
    QLineEdit* nameInput;
    QDateEdit* birthDateInput;  // QDateEdit cho ngày sinh
    QLineEdit* genderInput;
    QLineEdit* classNameInput;
    QLineEdit* majorInput;
    QLineEdit* khoaInput;
    QLineEdit* soKhoaHocInput;
    QLineEdit* sodienthoaiInput;
    QLineEdit* diachiInput;
};

// MainWindow class
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    // MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void onLoadData();
    void onLoadCSV();
    void onAddStudent();
    void onDeleteStudent();
    void onSortByID();
    void outData();
    void deleteRow(QTableWidget* tableWidget, int row);
    void searchStudent(const QString& searchText);
    void onLogin();
    void togglePasswordVisibility(bool checked);
    void showStudentManagementPage();
private:
    QTableWidget* studentTable;
    StudentList studentList;
    Ui::MainWindow *ui;
    QLineEdit *usernameLineEdit;  // Đảm bảo khai báo ở đây
    QLineEdit *passwordLineEdit;  // Đảm bảo khai báo ở đây
    QPushButton *loginButton;      // Đảm bảo khai báo ở đây
    QLabel *statusLabel;
    QCheckBox *showPasswordCheckBox;
};

#endif // MAINWINDOW_H

