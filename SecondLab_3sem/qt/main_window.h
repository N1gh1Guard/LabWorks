#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QGroupBox>
#include <QFileDialog>

#include "../hpp/AlphabetIndex.hpp"
#include "../hpp/GuiFunctions.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Database/Cache operations
    void onPutClicked();
    void onGetClicked();
    void onEraseClicked();
    void onClearDBClicked();
    void onClearCacheClicked();
    void onRefreshKeysClicked();

    // Alphabet Index operations
    void onBuildIndexClicked();
    void onLookupWordClicked();
    void onShowPagesClicked();
    void onShowPointerClicked();
    void onLoadFileClicked();

private:
    void setupDatabaseTab();
    void setupAlphabetTab();
    QWidget* createDatabaseWidget();
    QWidget* createAlphabetWidget();
    void updateKeyList();
    void updateStats();

    // Database/Cache UI
    QLineEdit *keyEdit_;
    QLineEdit *valueEdit_;
    QTextEdit *resultText_;
    QListWidget *keysList_;
    QLabel *dbSizeLabel_;
    QLabel *cacheSizeLabel_;

    // Alphabet Index UI
    QTextEdit *textInput_;
    QSpinBox *pageSizeSpin_;
    QComboBox *pageTypeCombo_;
    QLineEdit *wordSearchEdit_;
    QTextEdit *indexResultText_;

    // Managers
    DatabaseManager<std::string, std::string> *dbManager_;
    AlphabetIndexManager *indexManager_;
};

#endif // MAIN_WINDOW_H

