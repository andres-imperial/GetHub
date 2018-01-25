#include <iostream>
#include <vector>
#include <cstring>
#include <iterator>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>
#include <QTreeWidget>
#include <QApplication>
#include "gethub.h"
#include "ui_gethub.h"
#include "restclient-cpp/restclient.h"
#include "json.hpp"

enum{
    Url = Qt::UserRole,
    Api = Qt::UserRole+1,
    Download = Qt::UserRole+2
};

GetHub::GetHub(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GetHub)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(1);
}

GetHub::~GetHub()
{
    delete ui;
}

void GetHub::on_findButton_clicked()
{
    // Get user input and call function to make api request
    loadTextFile("Searching . . .");
    ui->listWidget->clear();
    qApp->processEvents();
    QString searchString = ui->lineEdit->text();
    std::vector<Repo> repoList = GetRepos(searchString);

    if (repoList.size() == 0) {
        // Error nothing found
        loadTextFile("Error: Nothing found for " + searchString);
        ui->treeWidget->clear();
    }

    // List Repos
    loadTextFile("Displaying repos for " + searchString);
    ListRepos(repoList);

}

std::vector<Repo> GetHub::GetRepos(QString username)
{
    // Create vector to hold api results
    std::vector<Repo> repoList;
    // Make get request with api
    QString api = "https://api.github.com/users/" + username + "/repos";
    RestClient::Response r = RestClient::get(api.toUtf8().constData());

    // Make sure vector is empty
    if (repoList.size() != 0) {
        repoList.clear();
    }

    // Check for valid response
    if (r.code == 200) {
        nlohmann::json j = nlohmann::json::parse(r.body);

        for (uint i = 0; i < j.size(); ++i) {
            // Create repo object and add it to repoList
            Repo temp;
            temp.setUrl(QString::fromStdString(j[i]["html_url"]).remove("\""));
            temp.setName(QString::fromStdString(j[i]["name"]).remove("\""));
            temp.setApi(QString::fromStdString(j[i]["contents_url"]).remove("\""));
            temp.chop(7);
            repoList.push_back(temp);
        }

        // Return a list of found repos
        return repoList;

    }
    else {
        // Error code, return empty vector.
        return repoList;
    }

}

void GetHub::ListRepos(std::vector<Repo> list)
{
    for (uint i = 0; i < list.size(); ++i) {
        // Add repos to list and set their data to url links
        ui->listWidget->addItem(list[i].name());
        ui->listWidget->item(i)->setData(Url, list[i].url());
        ui->listWidget->item(i)->setData(Api, list[i].api());
    }

}

void GetHub::loadTextFile()
{
    QFile inputFile(":/input.txt");
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();

    ui->infoLine->setText(line);
}

void GetHub::loadTextFile(QString myString)
{
    ui->infoLine->setText(myString);
}

void GetHub::on_lineEdit_returnPressed()
{
    ui->findButton->click();
}

void GetHub::on_listWidget_itemClicked(QListWidgetItem *item)
{
    // If repo tree does not already exist create one
    if (!ui->treeWidget->findItems(item->text(), 0).size()) {
        // Create QTreeWidgetItem from QListWidgetItem
        loadTextFile("Building tree . . .");
        qApp->processEvents();
        QTreeWidgetItem *newItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList((QString)item->text()));
        newItem->setData(0, Api, item->data(Api));
        newItem->setData(0, Url, item->data(Url));
        // Get repo tree for clicked item
        GetRepoTree(newItem);
        // Display recieved tree
        ui->treeWidget->insertTopLevelItem(0, newItem);
        loadTextFile("Tree built");
    }
}

void GetHub::GetRepoTree(QTreeWidgetItem *item)
{
    // Update app to stay responsive when building large trees
    qApp->processEvents();
    // Make Api call
    RestClient::Response r = RestClient::get(item->data(0,Api).toString().toUtf8().constData());

    // Check for valid response
    if (r.code == 200) {
        // Parse response into json object
        nlohmann::json j = nlohmann::json::parse(r.body);
        QList<QTreeWidgetItem *> items;
        // Cycle through objects and append to tree list
        for (uint i = 0; i < j.size(); ++i) {
            items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString::fromStdString(j[i]["name"]).remove("\""))));
            // If object is a directory do recursive call on GetRepoTree
            if (j[i]["type"] == "dir") {
                items.last()->setData(0, Api, QString::fromStdString(j[i]["url"]).remove("\""));
                items.last()->setData(0, Url, QString::fromStdString(j[i]["html_url"]).remove("\""));
                GetRepoTree(items.last());
            }
            // If object is file save download url
            else if (j[i]["type"] == "file") {
                items.last()->setData(0, Download, QString::fromStdString(j[i]["download_url"]).remove("\""));
                items.last()->setData(0, Url, QString::fromStdString(j[i]["html_url"]).remove("\""));
            }
        }
        // Add list to calling parent's children
        item->addChildren(items);
    }

}

void GetHub::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // Display item that was clicked and open default browser to item's url
    loadTextFile(item->data(column, Url).toString());
    QDesktopServices::openUrl(item->data(column, Url).toUrl());
}

void GetHub::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QString link = item->data(column, Download).toString();
    RestClient::Response r = RestClient::get(link.toUtf8().constData());

    if (r.code == 200) {
        loadTextFile(QString::fromStdString(r.body));
        QFile saveFile("../../Desktop/" + item->text(column));
        saveFile.open(QIODevice::WriteOnly);
        saveFile.write(r.body.c_str());
        saveFile.close();

    }
}
