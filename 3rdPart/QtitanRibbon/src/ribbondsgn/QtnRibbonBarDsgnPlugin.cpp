/****************************************************************************
**
** Qtitan Library by Developer Machines (Microsoft-Ribbon implementation for Qt.C++)
** 
** Copyright (c) 2009-2021 Developer Machines (https://www.devmachines.com)
**           ALL RIGHTS RESERVED
** 
**  The entire contents of this file is protected by copyright law and
**  international treaties. Unauthorized reproduction, reverse-engineering
**  and distribution of all or any portion of the code contained in this
**  file is strictly prohibited and may result in severe civil and 
**  criminal penalties and will be prosecuted to the maximum extent 
**  possible under the law.
**
**  RESTRICTIONS
**
**  THE SOURCE CODE CONTAINED WITHIN THIS FILE AND ALL RELATED
**  FILES OR ANY PORTION OF ITS CONTENTS SHALL AT NO TIME BE
**  COPIED, TRANSFERRED, SOLD, DISTRIBUTED, OR OTHERWISE MADE
**  AVAILABLE TO OTHER INDIVIDUALS WITHOUT WRITTEN CONSENT
**  AND PERMISSION FROM DEVELOPER MACHINES
**
**  CONSULT THE END USER LICENSE AGREEMENT FOR INFORMATION ON
**  ADDITIONAL RESTRICTIONS.
**
****************************************************************************/
#include <QStyle>
#include <QApplication>
#include <QContextMenuEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/QDesignerWidgetFactoryInterface>
#include <QtDesigner/QDesignerMetaDataBaseInterface>
#include <QtDesigner/QDesignerFormWindowManagerInterface>
#include <QtDesigner/QDesignerPropertySheetExtension>
#include <QtDesigner/QDesignerDnDItemInterface>

#include "QtnRibbonDsgnPlugins.h"
#include "QtnRibbonDsgnContainer.h"
#include "QtnRibbonDsgnContainer.h"

#include "QtnRibbonDsgnTaskMenu.h"
#include "QtnRibbonMainWindow.h"
#include "QtnCommandDsgn.h"
#include "QtnRibbonBarDsgnPlugin.h"
#include "QtnRibbonPage.h"
#include "QtnRibbonBar.h"
#include "../src/ribbon/QtnRibbonTabBar.h"


QTITAN_USE_NAMESPACE


/* DesignerRibbonBar */
DesignerRibbonBar* DesignerRibbonBar::createRibbonBar(QWidget* parent)
{
    class RibbonBarFake : public DesignerRibbonBar
    {
    public:
        RibbonBarFake(QWidget* parent) : DesignerRibbonBar(parent) {}
    public:
        virtual const QMetaObject* metaObject() const { return RibbonBar::metaObject(); }
        virtual void* qt_metacast(const char *_clname) { return RibbonBar::qt_metacast(_clname); }
    };

    return new RibbonBarFake(parent);
}

DesignerRibbonBar::~DesignerRibbonBar()
{
}

DesignerRibbonBar::DesignerRibbonBar(QWidget* parent)
    : RibbonBar(parent)
{
    m_removeRibbonBar = new QAction(tr("Remove Ribbon Bar"), this);
    connect(m_removeRibbonBar, SIGNAL(triggered()), this, SLOT(slotRemoveRibbonBar()));

    m_addRibbonPage = new QAction(tr("Insert page"), this);
    connect(m_addRibbonPage, SIGNAL(triggered()), this, SLOT(slotAddNewPage()));

    m_removeRibbonPage = new QAction(tr("Delete current page"), this);
    connect(m_removeRibbonPage, SIGNAL(triggered()), this, SLOT(slotRemoveCurrentPage()));

    RibbonTabBar* ribbonTabBar = this->findChild<RibbonTabBar*>();
    Q_ASSERT(ribbonTabBar != Q_NULL);
    installEventFilter(new RibbonTabBarFilter(ribbonTabBar));
}

void DesignerRibbonBar::slotRemoveRibbonBar()
{
    QDesignerFormWindowInterface* fw = QDesignerFormWindowInterface::findFormWindow(this);
    Q_ASSERT(fw != 0);

    DeleteRibbonBarCommand* cmd = new DeleteRibbonBarCommand(fw);
    cmd->init(this);
    fw->commandHistory()->push(cmd);
}

void DesignerRibbonBar::slotAddNewPage()
{
    QDesignerFormWindowInterface* fw = QDesignerFormWindowInterface::findFormWindow(this);
    Q_ASSERT(fw != 0);

    RibbonPage* ribbonPage = qobject_cast<RibbonPage*>(fw->core()->widgetFactory()->createWidget(create_qtitan_className(QStringLiteral("RibbonPage")), this));
    Q_ASSERT(ribbonPage != 0);

    AddRibbonPageCommand* cmd = new AddRibbonPageCommand(fw);
    cmd->init(this, ribbonPage);
    fw->commandHistory()->push(cmd);
}

void DesignerRibbonBar::slotRemoveCurrentPage()
{
    QDesignerFormWindowInterface* fw = QDesignerFormWindowInterface::findFormWindow(this);
    Q_ASSERT(fw != 0);

    DeleteRibbonPageCommand* cmd = new DeleteRibbonPageCommand(fw);
    cmd->init(this);
    fw->commandHistory()->push(cmd);
}

bool DesignerRibbonBar::event(QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick)
        return true;
    return RibbonBar::event(event);
}

void DesignerRibbonBar::contextMenuEvent(QContextMenuEvent* event)
{
    event->accept();
    update();
    QMenu menu;
    menu.addAction(m_removeRibbonBar);
    menu.addSeparator();
    menu.addAction(m_addRibbonPage);

    if (pageCount() > 0)
        menu.addAction(m_removeRibbonPage);

    menu.exec(event->globalPos());
}

void DesignerRibbonBar::resizeEvent(QResizeEvent* event)
{
    RibbonBar::resizeEvent(event);

    QDesignerFormWindowInterface* form = QDesignerFormWindowInterface::findFormWindow(this);
    if (form == Q_NULL)
        return;
    if (QDesignerPropertySheetExtension *sheet =
        qt_extension<QDesignerPropertySheetExtension*>(form->core()->extensionManager(), this))
    {
        sheet->setChanged(sheet->indexOf(QStringLiteral("geometry")), false);
    }
}


/* DsgnRibbonBarPlugin */
DsgnRibbonBarPlugin::DsgnRibbonBarPlugin(QObject *parent)
    : QObject(parent)
    , m_core(Q_NULL)
    , m_managerFilter(Q_NULL)
{
    m_initialized = false;
}

void DsgnRibbonBarPlugin::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != Q_NULL);

    manager->registerExtensions(new DsgnRibbonTaskMenuFactory(manager), QStringLiteral("QDesignerInternalTaskMenuExtension"));
    manager->registerExtensions(new DsgnRibbonBarContainerFactory(manager), Q_TYPEID(QDesignerContainerExtension));

    m_initialized = true;
    m_core = core;
    QObject::connect(core->formWindowManager(), SIGNAL(formWindowAdded(QDesignerFormWindowInterface*)), this, SLOT(formWindowAdded(QDesignerFormWindowInterface*)));
}

bool DsgnRibbonBarPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* DsgnRibbonBarPlugin::createWidget(QWidget* parent)
{
    DesignerRibbonBar* result = DesignerRibbonBar::createRibbonBar(parent);
    result->setTitleBarVisible(false);
    result->setAcceptDrops(true);
    connect(result, SIGNAL(currentPageIndexChanged(int)), this, SLOT(currentPageChanged(int)));
    return result;
}

QString DsgnRibbonBarPlugin::name() const
{
    return create_qtitan_className(QStringLiteral("RibbonBar"));
}

QString DsgnRibbonBarPlugin::group() const
{
    return QStringLiteral("Developer Machines (Components)");
}

QIcon DsgnRibbonBarPlugin::icon() const
{
    return QIcon(QStringLiteral(":res/ribbon_icon.png"));
}

QString DsgnRibbonBarPlugin::toolTip() const
{
    return tr("Advanced RibbonBar for Qt.C++");
}

QString DsgnRibbonBarPlugin::whatsThis() const
{
    return tr("Advanced RibbonBar for Qt.C++");
}

bool DsgnRibbonBarPlugin::isContainer() const
{
    return false;
}

QString DsgnRibbonBarPlugin::domXml() const
{
    QString className = create_qtitan_className(QStringLiteral("RibbonBar"));
    return QStringLiteral("<ui language=\"c++\">\n"
        " <widget class=\"" "%1" "\" name=\"ribbonBar\"/>\n"
        " <customwidgets>\n"
        "   <customwidget>\n"
        "       <class>" "%2" "</class>\n"
        "       <extends>QMenuBar</extends>\n"
        "       <addpagemethod>addPage</addpagemethod>\n"
        "   </customwidget>\n"
        " </customwidgets>\n"
        "</ui>\n").arg(className).arg(className);
}

QString DsgnRibbonBarPlugin::includeFile() const
{
    return QStringLiteral("DevMachines/QtitanRibbon");
}

void DsgnRibbonBarPlugin::currentPageChanged(int index)
{
    Q_UNUSED(index);
    RibbonBar* ribbonBar  = qobject_cast<RibbonBar*>(sender());
    Q_ASSERT(ribbonBar != Q_NULL);
    if (QDesignerFormWindowInterface* form = QDesignerFormWindowInterface::findFormWindow(ribbonBar)) 
    {
        if (RibbonPage* ribbonPage = ribbonBar->page(index))
        {
            form->clearSelection();
            form->selectWidget(ribbonPage);
        }
    }
}

void DsgnRibbonBarPlugin::selectionChanged()
{
    /*
    QDesignerFormWindowInterface* form = QDesignerFormWindowInterface::findFormWindow(sender());
    if (RibbonMainWindow* mainWindow = qobject_cast<RibbonMainWindow*>(form->mainContainer()))
    { 
        if (RibbonBar* ribbonBar = qobject_cast<RibbonBar*>(mainWindow->menuWidget()))
        {
            int index = ribbonBar->currentPageIndex();
            if (index != -1)
            {
                form = QDesignerFormWindowInterface::findFormWindow(ribbonBar);
                RibbonPage* ribbonPage = ribbonBar->getPage(index);
                Q_ASSERT(ribbonPage != Q_NULL);
                form->selectWidget(ribbonPage);
            }
        }
    }
    */
}

void DsgnRibbonBarPlugin::formWindowAdded(QDesignerFormWindowInterface* formWindow)
{
    QObject::connect(formWindow, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    QObject::connect(formWindow, SIGNAL(widgetManaged(QWidget*)), this, SLOT(widgetManaged(QWidget*)));
    m_managerFilter = new DsgnFormRibbonManagerFilter(m_core, formWindow, this);
    qApp->installEventFilter(m_managerFilter);
}

void DsgnRibbonBarPlugin::formWindowRemoved(QDesignerFormWindowInterface* formWindow)
{
    QObject::disconnect(formWindow, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    QObject::disconnect(formWindow, SIGNAL(widgetManaged(QWidget*)), this, SLOT(widgetManaged(QWidget*)));
}

void DsgnRibbonBarPlugin::widgetManaged(QWidget* widget)
{
    if (QLatin1String(widget->metaObject()->className()) == create_qtitan_className(QStringLiteral("RibbonBar")))
    {
        QDesignerFormWindowInterface* formWindow = static_cast<QDesignerFormWindowInterface *>(sender());
        QDesignerFormEditorInterface* core = formWindow->core();
        QDesignerContainerExtension* container = qt_extension<QDesignerContainerExtension*>(core->extensionManager(), formWindow->mainContainer());
        formWindow->unmanageWidget(widget);

        QUndoStack* stack = formWindow->commandHistory();
        if (!stack->isClean())
        {
            //This code check the InsertWidget command on the stack.
            const QUndoCommand* command = stack->command(stack->index());
            if (command->childCount() == 0)
                return;
        }

        if (qobject_cast<QMainWindow *>(formWindow->mainContainer()) == 0)
        {
                QMessageBox::critical(
                    formWindow->mainContainer(),
                    tr("Can't add Ribbon Bar"), 
                    tr("You can add Ribbon Bar onto RibbonMainWindow or QMainWindow only."));
                widget->deleteLater();
                return;
        }

        for (int i = 0; i < container->count(); ++i)
        {
            QWidget* w = container->widget(i);
            if (QLatin1String(w->metaObject()->className()) == create_qtitan_className(QStringLiteral("RibbonBar")) ||
                QLatin1String(w->metaObject()->className()) == QStringLiteral("QDesignerMenuBar"))
            {
                QMessageBox::critical(
                    formWindow->mainContainer(),
                    tr("Can't add Ribbon Bar"), 
                    tr("Only one instance of the Ribbon Bar can be adding to the main form."));
                widget->deleteLater();
                return;
            }
        }

        container->addWidget(widget);
        formWindow->core()->metaDataBase()->add(widget);
    }
}

/* DsgnFormRibbonManagerFilter */
DsgnFormRibbonManagerFilter::DsgnFormRibbonManagerFilter(QDesignerFormEditorInterface* core, QDesignerFormWindowInterface* formWindow, QObject* parent)
    : QObject(parent)
    , m_core(core)
    , m_formWindow(formWindow)
{
}

/* Please see original QDesignerMimeData declaration */
class  QDesignerMimeData_fake : public QMimeData {
    Q_OBJECT
public:
    typedef QList<QDesignerDnDItemInterface *> QDesignerDnDItemsEx;
    virtual ~QDesignerMimeData_fake();
    const QDesignerDnDItemsEx &items() const { return m_items; }
    void acceptEvent(QDropEvent *e) const;
private:
    const QDesignerDnDItemsEx m_items;
};

bool DsgnFormRibbonManagerFilter::eventFilter(QObject* obj, QEvent* event)
{
    if (!obj->isWidgetType())
        return QObject::eventFilter(obj, event);
//    if (qobject_cast<QMainWindow*>)

    RibbonBar* targetRibbonBar = qobject_cast<RibbonBar *>(obj);
    RibbonPage* targetRibbonPage = qobject_cast<RibbonPage *>(obj);
    RibbonGroup* targetRibbonGroup = qobject_cast<RibbonGroup *>(obj);
    const bool isTargetRibbonElement = targetRibbonBar || targetRibbonPage || targetRibbonGroup;

    if (!isTargetRibbonElement || qobject_cast<RibbonMainWindow*>(obj))
    {
//        event->accept();
        return false;
    }

    switch (event->type())
    {
        case QEvent::MouseMove:
        {
            if (isTargetRibbonElement && static_cast<QMouseEvent*>(event)->buttons() == Qt::LeftButton)
                return true;
        }
        break;
        case QEvent::DragMove:
        case QEvent::DragEnter:
        {
            if (QDragEnterEvent* dEvent = static_cast<QDragEnterEvent*>(event))
            {
                const QMetaObject* mObject = dEvent->mimeData()->metaObject();
                const QString name = mObject ? QLatin1String(mObject->className()) : QString();
                if (name == QLatin1String("qdesigner_internal::ActionRepositoryMimeData"))
                    return false;

                if (const QDesignerMimeData_fake* mimedata = static_cast<const QDesignerMimeData_fake *>(dEvent->mimeData()))
                {
                    bool isSourceRibbonBar = false;
                    bool isSourceRibbonPage = false;
                    bool isSourceRibbonGroup = false;
                    for (QList<QDesignerDnDItemInterface *>::ConstIterator it = mimedata->items().constBegin(); it != mimedata->items().constEnd(); ++it)
                    {
                        QWidget* w = (*it)->widget();
                        if (w == 0)
                            w = (*it)->decoration();

                        QList<QWidget *> allWidgets = w->findChildren<QWidget*>();
                        if (allWidgets.size() == 0)
                            return false;
                        w = allWidgets.front();

                        isSourceRibbonBar = isSourceRibbonBar || qobject_cast<RibbonBar *>(w);
                        isSourceRibbonPage = isSourceRibbonPage || qobject_cast<RibbonPage *>(w);
                        isSourceRibbonGroup = isSourceRibbonGroup || qobject_cast<RibbonGroup *>(w);
                    }

                    const bool isSourceRibbonElement = isSourceRibbonBar || isSourceRibbonPage || isSourceRibbonGroup;

                    if (!isSourceRibbonElement && !isTargetRibbonElement)
                        return false;

                    if ((targetRibbonBar && isSourceRibbonPage) || (targetRibbonPage && isSourceRibbonGroup))
                    {
                        dEvent->accept();
                        return true;
                    }
                    dEvent->ignore();
                    return true;
                }
            }
        }
        break;
        case QEvent::Drop:
        {
            if (QDropEvent* dEvent = static_cast<QDropEvent*>(event))
            {
                const QMetaObject* mObject = dEvent->mimeData()->metaObject();
                const QString name = mObject ? QLatin1String(mObject->className()) : QString();
                if (name == QLatin1String("qdesigner_internal::ActionRepositoryMimeData"))
                    return false;

                const QDesignerMimeData_fake* mimedata = static_cast<const QDesignerMimeData_fake *>(dEvent->mimeData());
                if (mimedata)
                {
                    RibbonBar* sourceRibbonBar = Q_NULL;
                    RibbonPage* sourceRibbonPage = Q_NULL;
                    RibbonGroup* sourceRibbonGroup = Q_NULL;

                    //bool isSourceLayout = false;

                    for (QList<QDesignerDnDItemInterface *>::ConstIterator it = mimedata->items().constBegin(); it != mimedata->items().constEnd(); ++it)
                    {
                        QWidget* w = (*it)->widget();
                        if (w == 0)
                            w = (*it)->decoration();

                        QList<QWidget *> allWidgets = w->findChildren<QWidget*>();
                        if (allWidgets.size() == 0)
                            return false;
                        w = allWidgets.front();

                        if (sourceRibbonBar == Q_NULL && qobject_cast<RibbonBar *>(w))
                            sourceRibbonBar = qobject_cast<RibbonBar *>(w);
                        if (sourceRibbonPage == Q_NULL && qobject_cast<RibbonPage *>(w))
                            sourceRibbonPage = qobject_cast<RibbonPage *>(w);
                        if (sourceRibbonGroup == Q_NULL && qobject_cast<RibbonGroup *>(w))
                            sourceRibbonGroup = qobject_cast<RibbonGroup *>(w);

                        //isSourceLayout = isSourceLayout || qobject_cast<QLayout *>(w);
                    }
                    const bool isSourceRibbonElement = sourceRibbonBar || sourceRibbonPage || sourceRibbonGroup;

                    if (!isSourceRibbonElement && !isTargetRibbonElement)
                        return false;

                    if (targetRibbonBar && sourceRibbonPage)
                    {
                        if (QDesignerFormWindowInterface* fw = QDesignerFormWindowInterface::findFormWindow((QObject*)targetRibbonBar))
                        {
                            AddRibbonPageCommand* cmd = new AddRibbonPageCommand(fw);
                            cmd->init(targetRibbonBar, sourceRibbonPage);
                            cmd->redo();
                        }
                        return true;
                    }
                    else if (targetRibbonPage && sourceRibbonGroup)
                    {
                        if (QDesignerFormWindowInterface* fw = QDesignerFormWindowInterface::findFormWindow(targetRibbonPage))
                        {
                            AddRibbonGroupCommand* cmd = new AddRibbonGroupCommand(fw);
                            cmd->init(targetRibbonPage, sourceRibbonGroup);
                            cmd->redo();
                        }
                    }
                    return true;
                }
            }
        }
        break;
        default:
            break;
    }
    return QObject::eventFilter(obj, event);
}
