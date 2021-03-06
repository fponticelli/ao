#include <QDebug>
#include <QKeyEvent>

#include "gui/script.hpp"

void Script::keyPressEvent(QKeyEvent* e)
{
    // By default, we'll accept the event here.
    // If it doesn't meet any of our conditions, then we'll set it as
    // ignored in the final else statement, then call the default key
    // press handler for QTextEdit.
    e->setAccepted(true);

    auto c = textCursor();
    if (e->key() == Qt::Key_Tab)
    {
        if (c.hasSelection())
        {
            auto a = c.selectionStart();
            auto b = c.selectionEnd();

            c.setPosition(a);
            c.beginEditBlock();
            while (true)
            {
                c.movePosition(QTextCursor::StartOfLine);
                c.insertText("  ");
                c.movePosition(QTextCursor::Down);

                if (c.position() == a || c.position() > b)
                {
                    break;
                }
                a = c.position();
            }
            c.endEditBlock();
        }
        else
        {
            insertPlainText("  ");
        }
    }
    else if (e->key() == Qt::Key_Backtab && c.hasSelection())
    {
        auto a = c.selectionStart();
        auto b = c.selectionEnd();

        c.setPosition(a);
        c.beginEditBlock();
        while (true)
        {
            c.movePosition(QTextCursor::StartOfLine);
            for (unsigned i=0; i < 2; ++i)
            {
                c.movePosition(QTextCursor::Right,
                               QTextCursor::KeepAnchor);
                if (c.selectedText() == " ")
                {
                    c.removeSelectedText();
                }
            }
            c.movePosition(QTextCursor::Down);

            if (c.position() == a || c.position() > b)
            {
                break;
            }
            a = c.position();
        }
        c.endEditBlock();
    }
    else if (e->key() == Qt::Key_Return && !c.hasSelection())
    {
        auto c = textCursor();

        // Count the number of leading spaces on this line
        c.movePosition(QTextCursor::StartOfLine);
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        while (*c.selectedText().rbegin() == ' ')
        {
            c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        }

        // Then match the number of leading spaces
        insertPlainText("\n" + QString(c.selectedText().length() - 1, ' '));
    }
    else
    {
        e->setAccepted(false);
        QTextEdit::keyPressEvent(e);
    }
}
