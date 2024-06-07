#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <queue>
#include <string>
#include <vector>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalculateButtonClicked();

private:
    Ui::MainWindow *ui;

    int chess2num(std::string str);
    std::string num2chess(int a);
    void bfs(std::queue<std::pair<int, int>>& q, int d[8][8], int dx[], int dy[], int steps, bool multiple_steps);
    void way(int d[8][8], int ex, int ey, std::string& str, int dx[], int dy[], int steps, bool multiple_steps);
    void updateChessBoard(int d[8][8], int sx, int sy, int ex, int ey, const std::vector<std::pair<int, int>>& path, int mode);
    std::vector<std::string> parsePath(const std::string& path);
    std::string toChessNotation(int x, int y);
};

#endif // MAINWINDOW_H
