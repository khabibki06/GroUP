#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QTextStream>
#include <QPrinter>
#include <QDialog>
#include <QPrintDialog>
#include <QFontDialog>
#include <QShortcut>

namespace Ui {
class TextEditor;
}

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit TextEditor(QWidget *parent = 0);
    void TextEditor::openFile(QString fileName);
    ~TextEditor();

private slots:
    void newDocument();

    void open();

    void save();

    void saveAs();

    void print();

    void exit();

    void copy();

    void cut();

    void paste();

    void undo();

    void redo();

    void selectFont();

//    void setFontBold(bool bold);

//    void setFontUnderline(bool underline);

//    void setFontItalic(bool italic);

//    void about();

private:
    Ui::TextEditor *ui;
    QString currentFile;
};

#endif // TEXTEDITOR_H
