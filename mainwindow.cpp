/*
Nguyễn Văn Quốc
MSSV: 237480104020
CHƯƠNG TRÌNH QUẢN LÝ SINH VIÊN
*/
#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTableWidget>
#include <QWidget>
#include <QFile>
#include <QDebug>
#include <algorithm>
#include <QMainWindow>
#include <iostream>
#include <QSignalMapper>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QFont>
#include <QScreen>
#include <QPixmap>
#include <QPalette>
#include <QStackedWidget>

// Triển khai các phương thức của StudentList
void StudentList::append(const Student& student) {
    Node* newNode = new Node(student);
    if (!head) head = newNode;
    else {
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    students.push_back(student);
}

// Đọc danh sách sinh viên từ file CSV
void StudentList::inputStudentListFromCSV(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Lỗi", "Không thể mở file!");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() == 11) {
            Student student;
            student.id = fields[0].trimmed();
            student.name = fields[1].trimmed();
            student.birthDate = fields[2].trimmed();
            student.gender = fields[3].trimmed();
            student.className = fields[4].trimmed();
            student.major = fields[5].trimmed();
            student.khoa = fields[6].trimmed();
            student.soKhoaHoc = fields[7].trimmed();
            student.sdt = fields[8].trimmed();
            student.diachi = fields[9].trimmed();
            append(student);
        } else {
            qDebug() << "Invalid line, not enough fields!";
        }
    }
    file.close();
}

void StudentList::outputStudentListToCSV(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Lỗi", "Không thể mở file để ghi!");
        return;
    }
    QTextStream out(&file);


    // Lặp qua danh sách sinh viên và ghi vào file
    for (const Student& student: students) {
        out << student.id << ","
            << student.name << ","
            << student.birthDate << ","
            << student.gender << ","
            << student.className << ","
            << student.major << ","
            << student.khoa << ","
            << student.soKhoaHoc << ","
            << student.sdt << ","
            << student.diachi << "\n";
    }
    file.close();
    QMessageBox::information(nullptr, "Thành công", "Xuất danh sách sinh viên thành công!");
}


// In danh sách sinh viên lên bảng
void StudentList::printStudentList(QTableWidget* tableWidget, MainWindow* mainWindow) {
    tableWidget->setRowCount(0);
    int row = 0;
    for (Node* temp = head; temp; temp = temp->next) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(temp->student.id));
        tableWidget->setItem(row, 1, new QTableWidgetItem(temp->student.name));
        tableWidget->setItem(row, 2, new QTableWidgetItem(temp->student.birthDate));
        tableWidget->setItem(row, 3, new QTableWidgetItem(temp->student.gender));
        tableWidget->setItem(row, 4, new QTableWidgetItem(temp->student.className));
        tableWidget->setItem(row, 5, new QTableWidgetItem(temp->student.major));
        tableWidget->setItem(row, 6, new QTableWidgetItem(temp->student.khoa));
        tableWidget->setItem(row, 7, new QTableWidgetItem(temp->student.soKhoaHoc));
        tableWidget->setItem(row, 8, new QTableWidgetItem(temp->student.sdt));
        tableWidget->setItem(row, 9, new QTableWidgetItem(temp->student.diachi));
        // Thêm nút xóa vào cột thứ 10
        QPushButton* deleteButtonRow = new QPushButton("Xóa");
        tableWidget->setCellWidget(row, 10, deleteButtonRow);
        deleteButtonRow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        // Kết nối nút xóa với hàm deleteRow trong MainWindow
        QObject::connect(deleteButtonRow, &QPushButton::clicked, [this, tableWidget, row, mainWindow]() {
            mainWindow->deleteRow(tableWidget, row);
        });
        row++;
    }
}

// Xóa sinh viên khỏi danh sách
bool StudentList::deleteStudent(const QString& id) {
    Node* temp = head;
    Node* prev = nullptr;
    while (temp) {
        if (temp->student.id == id) {
            if (prev) {
                prev->next = temp->next;
            } else {
                head = temp->next;
            }
            delete temp;
            return true;
        }
        prev = temp;
        temp = temp->next;
    }
    return false;
}

// Sắp xếp danh sách sinh viên theo MSSV
void StudentList::sortByID() {
    if (!head || !head->next) return;
    std::vector<Node*> nodes;
    for (Node* temp = head; temp; temp = temp->next) {
        nodes.push_back(temp);
    }
    std::sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {
        return a->student.id < b->student.id;
    });
    for (size_t i = 0; i < nodes.size() - 1; ++i) {
        nodes[i]->next = nodes[i + 1];
    }
    nodes.back()->next = nullptr;
    head = nodes.front();
}

// Dọn dẹp danh sách
void StudentList::clearList() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Tạo QLabel để hiển thị logo
    QLabel* logoLabel = new QLabel(this);
    //QPixmap logoPixmap("C:/Users/Administrator/Videos/QuanLySinhVien/hinhanh/logo.png"); DEBUG
    QPixmap logoPixmap("hinhanh/logo.png");

    // Đặt kích thước logo
    int logoSize = 150; // Kích thước vuông nhỏ cho logo (150x150 pixel)
    logoPixmap = logoPixmap.scaled(logoSize, logoSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignCenter);  // Canh giữa logo

    // Tạo các widget đăng nhập
    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Tên đăng nhập");
    usernameLineEdit->setMinimumWidth(100);
    usernameLineEdit->setMinimumHeight(35);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("Mật khẩu");
    passwordLineEdit->setMinimumWidth(100);
    passwordLineEdit->setMinimumHeight(35);

    loginButton = new QPushButton("Đăng nhập", this);
    statusLabel = new QLabel(this);
    loginButton->setMinimumWidth(100);
    loginButton->setMinimumHeight(35);
    loginButton->setStyleSheet("background-color: blue; color: white; font-size: 13px; font-weight: bold;");
    showPasswordCheckBox = new QCheckBox("Hiện mật khẩu", this);  // Khởi tạo checkbox

    QPushButton* exitFullScreenButton = new QPushButton("Thoát ứng dụng", this);
    exitFullScreenButton->setStyleSheet("background-color: red; color: white; font-size: 13px; font-weight: bold;");

    // Kết nối nút đăng nhập với slot
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLogin);
    connect(showPasswordCheckBox, &QCheckBox::toggled, this, &MainWindow::togglePasswordVisibility);  // Kết nối checkbox

    // Thiết lập layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(logoLabel);  // Thêm logo vào layout
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(showPasswordCheckBox);  // Thêm checkbox vào layout
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);
    layout->addWidget(exitFullScreenButton);
    connect(exitFullScreenButton, &QPushButton::clicked, this, &MainWindow::close);

    // Tạo một widget trung gian để chứa layout
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Đặt kích thước cho cửa sổ
    showFullScreen();
}

// Khởi tạo MainWindow
MainWindow::~MainWindow() {
    // Dọn dẹp tài nguyên nếu cần
}
void MainWindow::outData() {
    //QString filename = "C:/Users/Administrator/Videos/QuanLySinhVien/DATA/students.csv"; DEBUG
    QString filename = "DATA/students.csv";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Lỗi", "Không thể mở file để ghi!");
        return;
    }

    QTextStream out(&file);

    // Lặp qua từng hàng trong bảng và ghi vào file
    for (int row = 0; row < studentTable->rowCount(); ++row) {
        for (int col = 0; col < studentTable->columnCount(); ++col) {
            QTableWidgetItem* item = studentTable->item(row, col);
            if (item) {
                out << item->text();
            }
            if (col < studentTable->columnCount() - 1) {
                out << ",";
            }
        }
        out << "\n";
    }

    file.close();
    QMessageBox::information(this, "Thành công", "Đã lưu danh sách sinh viên vào file!");
}

// Hàm tải file CSV
void MainWindow::onLoadData() {
    // Đường dẫn đến file students.csv trong thư mục DATA
    //QString fileName = "C:/Users/Administrator/Videos/QuanLySinhVien/DATA/students.csv"; DEBUG
    QString fileName = "DATA/students.csv";
    if (!fileName.isEmpty()) {
        studentList.inputStudentListFromCSV(fileName);
        studentList.printStudentList(studentTable, this);
    }
    studentList.printStudentList(studentTable, this);
}

void MainWindow::onLoadCSV() {
    QString fileName = QFileDialog::getOpenFileName(this, "Mở file CSV", "", "CSV files (*.csv)");
    if (!fileName.isEmpty()) {
        qDebug() << "CSV file selected: " << fileName;
        studentList.inputStudentListFromCSV(fileName);
        qDebug() << "Data loaded from CSV, printing list...";
        studentList.printStudentList(studentTable, this);
    }
    qDebug() << "File name: " << fileName;
}


void MainWindow::onAddStudent() {
    // Tạo một đối tượng AddStudentDialog
    AddStudentDialog dialog(this);

    // Hiển thị hộp thoại và chờ người dùng nhập
    if (dialog.exec() == QDialog::Accepted) {
        // Tạo sinh viên mới từ dữ liệu nhập
        Student s;
        s.id = dialog.getId();
        s.name = dialog.getName();
        //s.birthDate = dialog.getBirthDate();
        s.birthDate = dialog.getBirthDate().toString("dd/MM/yyyy");  // Chuyển QDate thành chuỗi nếu cần
        s.gender = dialog.getGender();
        s.className = dialog.getClassName();
        s.major = dialog.getMajor();
        s.khoa = dialog.getKhoa();
        s.soKhoaHoc = dialog.getSoKhoaHoc();
        s.sdt = dialog.getSoDienThoai();
        s.diachi = dialog.getDiaChi();

        // Kiểm tra các trường có rỗng không
        if (s.id.isEmpty() || s.name.isEmpty() || s.birthDate.isEmpty() || s.gender.isEmpty() ||
            s.className.isEmpty() || s.major.isEmpty() || s.khoa.isEmpty() || s.soKhoaHoc.isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ thông tin!");
            return;
        }
        // Thêm sinh viên vào danh sách và cập nhật bảng
        studentList.append(s);
        studentList.printStudentList(studentTable, this);
    }
}

// Hàm xóa sinh viên
void MainWindow::onDeleteStudent() {
    QString id = QInputDialog::getText(this, "Xóa sinh viên", "Nhập MSSV cần xóa:");
    if (!id.isEmpty()) {
        if (studentList.deleteStudent(id)) {
            studentList.printStudentList(studentTable, this);
        } else {
            QMessageBox::warning(this, "Lỗi", "Không tìm thấy sinh viên với MSSV " + id);
        }
    }
}

// Hàm sắp xếp danh sách sinh viên
void MainWindow::onSortByID() {
    studentList.sortByID();
    studentList.printStudentList(studentTable, this);
}
void MainWindow::deleteRow(QTableWidget* tableWidget, int row) {
    if (row >= 0 && row < tableWidget->rowCount()) {
        tableWidget->removeRow(row);
    }
}

void MainWindow::searchStudent(const QString& searchText) {
    for (int row = 0; row < studentTable->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < studentTable->columnCount(); ++col) {
            QTableWidgetItem* item = studentTable->item(row, col);
            if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        studentTable->setRowHidden(row, !match);  // Ẩn hoặc hiển thị hàng dựa trên kết quả tìm kiếm
    }
}

//Login
void MainWindow::onLogin()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    // Kiểm tra thông tin đăng nhập (thay thế bằng logic thực tế)
    if (username == "admin" && password == "1234") {
        statusLabel->setText("Đăng nhập thành công!");
        showStudentManagementPage();
    } else {
        statusLabel->setText("Tên đăng nhập hoặc mật khẩu không đúng.");
    }
}
void MainWindow::togglePasswordVisibility(bool checked)
{
    if (checked) {
        passwordLineEdit->setEchoMode(QLineEdit::Normal);  // Hiện mật khẩu
    } else {
        passwordLineEdit->setEchoMode(QLineEdit::Password);  // Ẩn mật khẩu
    }
}
void MainWindow::showStudentManagementPage()
{
    showFullScreen();
    //this->setMinimumSize(1280, 780);
    setWindowTitle("Quản lý sinh viên - Nguyễn Văn Quốc");

    // Tạo bảng sinh viên
    studentTable = new QTableWidget(0, 11, this);  // Đảm bảo khai báo studentTable là biến thành viên
    QStringList headers = {"MSSV", "Họ và Tên", "Ngày sinh", "Giới tính", "Lớp", "Ngành", "Khoa", "Khóa", "Số điện thoại", "Địa chỉ", "Hành động"};
    studentTable->setHorizontalHeaderLabels(headers);

    // Định dạng tiêu đề
    QFont headerFont = studentTable->horizontalHeader()->font();
    headerFont.setBold(true);                 // In đậm
    headerFont.setPointSize(11);              // Đặt kích thước chữ lớn hơn
    studentTable->horizontalHeader()->setFont(headerFont); // Áp dụng font cho tiêu đề

    // Thiết lập font chữ cho bảng
    QFont font("Arial", 13);
    studentTable->setFont(font);


    // Tải dữ liệu từ CSV trước khi hiển thị
    onLoadData();
    // Đặt kích thước cột cho nút "Xóa"
    studentTable->setColumnWidth(8, 300); // Giả sử cột "Hành động" là cột thứ 9 (index 8)

    // Tạo QLineEdit cho ô tìm kiếm
    QLineEdit* searchLineEdit = new QLineEdit(this);
    searchLineEdit->setMinimumWidth(100); // Đặt chiều rộng tối thiểu
    searchLineEdit->setMinimumHeight(35);
    searchLineEdit->setPlaceholderText("Tìm kiếm sinh viên...");
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::searchStudent);

    // Tạo các nút chức năng
    QPushButton* loadButton = new QPushButton("Tải CSV", this);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadCSV);
    loadButton->setStyleSheet("background-color: green; color: white; font-size: 16px; font-weight: bold;");

    QPushButton* addButton = new QPushButton("Thêm sinh viên", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    addButton->setStyleSheet("background-color: green; color: white; font-size: 16px; font-weight: bold;");

    QPushButton* deleteButton = new QPushButton("Xóa sinh viên", this);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    deleteButton->setStyleSheet("background-color: green; color: white; font-size: 16px; font-weight: bold;");

    QPushButton* sortButton = new QPushButton("Sắp xếp theo MSSV", this);
    connect(sortButton, &QPushButton::clicked, this, &MainWindow::onSortByID);
    sortButton->setStyleSheet("background-color: green; color: white; font-size: 16px; font-weight: bold;");

    QPushButton* saveButton = new QPushButton("Lưu", this);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::outData);
    saveButton->setStyleSheet("background-color: blue; color: white; font-size: 16px; font-weight: bold;");

    QPushButton *exitFullScreenButton = new QPushButton("Thoát ứng dụng", this);
    exitFullScreenButton->setStyleSheet("background-color: red; color: white; font-size: 16px; font-weight: bold;"); // Tô màu nền


    // Thiết lập layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(searchLineEdit);
    layout->addWidget(studentTable);  // Thêm bảng sinh viên vào layout

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(sortButton);
    buttonLayout->addWidget(saveButton);
    layout->addLayout(buttonLayout);
    layout->addWidget(exitFullScreenButton);
    connect(exitFullScreenButton, &QPushButton::clicked, this, &MainWindow::close);
    // Tạo widget trung gian
    QWidget* container = new QWidget();
    container->setLayout(layout);
    setCentralWidget(container);

    // Tự động điều chỉnh kích thước cột
    studentTable->resizeColumnsToContents();
    studentTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
