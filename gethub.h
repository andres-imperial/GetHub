#ifndef GETHUB_H
#define GETHUB_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include "repo.h"

namespace Ui {
class GetHub;
}

class GetHub : public QWidget
{
    Q_OBJECT

public:
    explicit GetHub(QWidget *parent = 0);
    ~GetHub();

private slots:
    void on_findButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_lineEdit_returnPressed();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::GetHub *ui;
    void loadTextFile(QString);
    void loadTextFile();
    std::vector<Repo> GetRepos(QString);
    void ListRepos(std::vector<Repo> list);
    void GetRepoTree(QTreeWidgetItem *item);
};

#endif // GETHUB_H
