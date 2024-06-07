#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <queue>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("BFS"); // Установка заголовка окна

    // Установка значка в заголовке окна
    QIcon windowIcon(":/new/images/knight.png");
    this->setWindowIcon(windowIcon);

    connect(ui->calculateButton, &QPushButton::clicked, this, &MainWindow::onCalculateButtonClicked);


    ui->chessBoardTableWidget->setRowCount(8);
    ui->chessBoardTableWidget->setColumnCount(8);

    for (int i = 0; i < 8; ++i) {
        ui->chessBoardTableWidget->setRowHeight(i, 50);
        ui->chessBoardTableWidget->setColumnWidth(i, 50);
    }

    QStringList horizontalLabels;
    horizontalLabels << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";
    ui->chessBoardTableWidget->setHorizontalHeaderLabels(horizontalLabels);

    QStringList verticalLabels;
    verticalLabels << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
    ui->chessBoardTableWidget->setVerticalHeaderLabels(verticalLabels);
}

MainWindow::~MainWindow()
{
    delete ui;
}


int MainWindow::chess2num(string str) {
    //перевод координат из шахматных в численные: a -> 1, b -> 2
    return str[0] - 'a';
}

string MainWindow::num2chess(int a) {
    //перевод координат из численных в шахматные: 1 -> a, 2 -> b
    return string(1, 'a' + a);
}


string MainWindow::toChessNotation(int x, int y) {
    return num2chess(x) + to_string(y + 1);
}

string sub(const string &coord) {
    string result = coord;
    result[0] = toupper(result[0]);
    return result;
}



void MainWindow::bfs(queue<pair<int, int>>& q, int d[8][8], int dx[], int dy[], int steps, bool multiple_steps) {
    //breadt-first search
    while (!q.empty()) { //пока очередь не пуста
        int cx = q.front().first;
        int cy = q.front().second;
        q.pop();
        for (int i = 0; i < steps; i++) {   //steps - количество ходов, которые может сделать фигура
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && d[nx][ny] == -1) {
                //пока координаты пренадлежат [0, 8] и вершина не посещена
                d[nx][ny] = d[cx][cy] + 1; //увеличиваем значение вершины +1
                q.push({nx, ny}); //добавляем вершину в очередь
                if (!multiple_steps) break; //если фигура не конь, то нужно учесть ходы не единичной длины
                nx += dx[i];
                ny += dy[i];
            }
        }
    }
}

void MainWindow::way(int d[8][8], int ex, int ey, string& str, int dx[], int dy[], int steps, bool multiple_steps) {
    //поиск пути в противоположном порядке bfs
    while (d[ex][ey] != 0) {
        for (int i = 0; i < steps; i++) { //перебираем каждый шаг фигуры
            int tx = ex + dx[i];
            int ty = ey + dy[i];
            while (tx >= 0 && tx < 8 && ty >= 0 && ty < 8) { //пока рассматриваемые вершины попадают в доску 8x8
                if (d[ex][ey] - 1 == d[tx][ty]) { //если находим вершину, из которой пришли в текущую, то записываем ее в путь str
                    str = num2chess(tx) + to_string(ty + 1) + " " + str;
                    ex = tx;
                    ey = ty; // восстанавливаем вершину с конца
                    break;
                }
                if (!multiple_steps) break; //отсекаем коня т.к. все его ходы учтены
                tx += dx[i];
                ty += dy[i];
            }
            if (d[ex][ey] == 0) break; //если вернулись к начальной вершине -> конец алгоритма
        }
    }
}

void MainWindow::updateChessBoard(int d[8][8], int sx, int sy, int ex, int ey, const std::vector<std::pair<int, int>>& path, int mode) {
    // вывод доски
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem();
            if (d[i][j] != -1) {
                if (d[i][j] == 0) {
                    // Устанавливаем изображение фигуры
                    QString way;
                if (mode == 1) {
                    way = ":/new/images/knight.png";

                }else if (mode == 2){
                    way = ":/new/images/rook.png";
                }else if(mode == 3){
                    way = ":/new/images/queen.png";
                }
                    QIcon icon(way);  // Путь к изображению фигуры
                    item->setIcon(icon);
                    ui->chessBoardTableWidget->setIconSize(QSize(40, 40));  // Устанавливаем размер иконки
                } else {
                    item->setText(QString::number(d[i][j]));
                    item->setTextAlignment(Qt::AlignCenter);  // Центрирование текста
                    item->setForeground(Qt::black);  // Цвет текста чёрный
                }
                if (i == sx && j == sy) {
                    item->setBackground(Qt::green);  // Начальная позиция зелёная
                } else if (i == ex && j == ey) {
                    item->setBackground(Qt::red);  // Конечная позиция красная
                } else if (std::find(path.begin(), path.end(), std::make_pair(i, j)) != path.end()) {
                    item->setBackground(Qt::yellow);  // Промежуточные позиции жёлтые
                }
            } else {
                item->setText("");
            }
            ui->chessBoardTableWidget->setItem(i, j, item);
        }
    }
}





vector<string> MainWindow::parsePath(const string& path) {
    vector<std::string> result;
    istringstream ss(path);
    string position;
    while (ss >> position) {
        int x = chess2num(position.substr(0, 1));
        int y = stoi(position.substr(1, 1)) - 1;
        result.push_back(toChessNotation(x, y));
    }
    return result;
}

void MainWindow::onCalculateButtonClicked() {
    QString figure = ui->figureComboBox->currentText();
    QString startPos = ui->startPositionLineEdit->text();
    QString endPos = ui->endPositionLineEdit->text();

    int mode;
    if (figure == "Конь") {
        mode = 1;
    } else if (figure == "Ладья") {
        mode = 2;
    } else if (figure == "Ферзь") {
        mode = 3;
    } else {
        ui->resultTextEdit->setText("Неизвестная фигура.");
        return;
    }

    int sx = chess2num(startPos.toStdString().substr(0, 1)); //переводим начальные и конечные координаты в численные
    int sy = stoi(startPos.toStdString().substr(1, 1)) - 1;
    int ex = chess2num(endPos.toStdString().substr(0, 1));
    int ey = stoi(endPos.toStdString().substr(1, 1)) - 1;

    int d[8][8];
    fill(&d[0][0], &d[0][0] + sizeof(d) / sizeof(d[0][0]), -1);

    queue<pair<int, int>> q;
    d[sx][sy] = 0;
    q.push({sx, sy});

    int dx_knight[8] = {-2, -1, 1, 2, 2, 1, -1, -2}; //для коня описываюстся все ходы относительно его позиции
    int dy_knight[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    int dx_rook[4] = {1, 0, -1, 0}; //для ладьи и ферзя описываются только ходы, приводящие к перемещению на 1 клетку
    int dy_rook[4] = {0, 1, 0, -1};

    int dx_queen[8] = {1, 1, 1, 0, -1, -1, -1, 0};
    int dy_queen[8] = {1, 0, -1, -1, -1, 0, 1, 1};

    if (mode == 1) {
        //значение false/true позволяет нам отделить ладью и ферзя от коня, ведь для них описаны не все ходы
        bfs(q, d, dx_knight, dy_knight, 8, false);
    } else if (mode == 2) {
        bfs(q, d, dx_rook, dy_rook, 4, true);
    } else if (mode == 3) {
        bfs(q, d, dx_queen, dy_queen, 8, true);
    }
    //на этом моменте доска пронумерована
    string result_way = num2chess(ex) + to_string(ey + 1);
    if (mode == 1) {
        way(d, ex, ey, result_way, dx_knight, dy_knight, 8, false);
    } else if (mode == 2) {
        way(d, ex, ey, result_way, dx_rook, dy_rook, 4, true);
    } else if (mode == 3) {
        way(d, ex, ey, result_way, dx_queen, dy_queen, 8, true);
    }

    //на этом моменте строка result_way содержит восстановленный путь
    auto path = parsePath(result_way);

    // Создание строки с путём в формате "A4 -> C3 -> B5"
    std::string formattedPath;
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) {
            formattedPath += " -> ";
        }
        formattedPath += sub(path[i]);
    }

    ui->resultTextEdit->setText(QString::fromStdString(formattedPath));
    // Устанавливаем размер шрифта
    QFont font = ui->resultTextEdit->font();
    font.setPointSize(16);  // Устанавливаем размер шрифта, например, 14
    ui->resultTextEdit->setFont(font);

    // Преобразуем вектор координат обратно в пары координат
    std::vector<std::pair<int, int>> coordPath;
    for (const auto& pos : path) {
        int x = chess2num(pos.substr(0, 1));
        int y = std::stoi(pos.substr(1, 1)) - 1;
        coordPath.push_back({x, y});
    }
    updateChessBoard(d, sx, sy, ex, ey, coordPath, mode);
}
