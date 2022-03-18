#include "Widgets/Progresses.hpp"

#include <QStandardItemModel>
#include <qnamespace.h>

ProgressItem::ProgressItem(int max, QString title, QWidget* p)
    : QFrame(p)
{
    auto effect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(effect);
    _animation = new QPropertyAnimation(effect, "opacity");
    _animation->setDuration(200);
    _animation->setStartValue(0);
    _animation->setEndValue(1);
    _animation->setEasingCurve(QEasingCurve::InCubic);

    this->setWindowFlag(Qt::WindowType::Widget);
    this->setFixedSize(200, 40);

    _progress = new QProgressBar(this);

    _layout = new QVBoxLayout(this);
    this->setLayout(_layout);

    _title = new QLabel(title);
    _title->setFont(QFont("JetBrains Mono NL Thin", 7));

    connect(_progress, SIGNAL(valueChanged(int)), this, SIGNAL(progressChanged(int)));
    _progress->setRange(0, max);
    _progress->setFormat("");
    _progress->setMinimumSize(200-24, 12);
    _progress->setMaximumHeight(12);
    _progress->setAlignment(Qt::AlignHCenter & Qt::AlignBottom);

    this->layout()->setAlignment(Qt::AlignHCenter);
    this->layout()->addWidget(_title);
    this->layout()->addWidget(_progress);
}

ProgressItem::~ProgressItem()
{
    delete _progress;
    delete _title;
    delete _layout;
}

void
ProgressItem::forseComplete()
{
    diactivate();
    connect(this, SIGNAL(diactivated()), this, SIGNAL(completed()));
}

void
ProgressItem::diactivate()
{
    auto effect = new QGraphicsOpacityEffect();
    this->setGraphicsEffect(effect);
    QPropertyAnimation * a = new QPropertyAnimation(effect, "opacity");
    a->setDuration(1000);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a, SIGNAL(finished()), this, SIGNAL(diactivated()));
}

void
ProgressItem::activate()
{
    this->show();
    _animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void
ProgressItem::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::MouseButton::LeftButton) {
        Q_EMIT clicked();
    }
}

void ProgressItem::setTitle(const QString& t) { _title->setText(t); }

void ProgressItem::setProgress(int v) {
    _progress->setValue(v);
    if (v == _progress->maximum()) {
        diactivate();
        connect(this, SIGNAL(diactivated()), this, SIGNAL(completed()));
    }
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

pSetProgress::pSetProgress(QWidget *w, QMargins mrgns, QWidget * p)
    : QObject(p),
        _win(w),
        _margins(mrgns),
        _spacing(2)
{
    connect(this, SIGNAL(setProgress(int,int,int,QString)), this, SLOT(on_setProgress(int, int, int, QString)));
    connect(this, SIGNAL(windowResized(QResizeEvent*)), this, SLOT(reorganize()));
}

pSetProgress::~pSetProgress()
{
    for (auto p : _bars) {
        delete p;
    }
}

void
pSetProgress::reorganize()
{
    QList<ProgressItem*> active_bars;

    for (auto bar : _bars) {
        if (!bar->isHidden()) {
            active_bars.append(bar);
        }
    }
    struct {
        bool operator()(QWidget* a, QWidget* b) const { return a->pos().y() > b->pos().y(); };
    } posCheck;
    std::sort(active_bars.begin(), active_bars.end(), posCheck);

    int i = 0;
    for (auto bar : active_bars) {
        bar->move(NextPosition(i++));
    }
}

void
pSetProgress::on_barCompleted()
{
    ProgressItem* bar = dynamic_cast<ProgressItem*>(sender());
    _bars.remove(bar->objectName().toInt());
    delete bar;
    reorganize();
}

void
pSetProgress::hideBar()
{
    ProgressItem* bar = dynamic_cast<ProgressItem*>(sender());
    bar->diactivate();
    connect(bar, SIGNAL(diactivated()), bar, SLOT(hide()));
    connect(bar, SIGNAL(diactivated()), this, SLOT(reorganize()));
}

QPoint
pSetProgress::NextPosition(int i)
{
    const int winw = _win->size().width(),
              winh = _win->size().height(),
              itmw = 200, itmh = 40; // TODO
    QPoint pos = {winw - itmw - _margins.right(),
                  winh - itmh - _margins.bottom() };

    if (i > 0) {
        pos.ry() -= i*(itmh + _spacing);
    }

    return pos;
}

int
pSetProgress::activeBars()
{
    int i = 0;
    for (auto bar : _bars) {
        if (!bar->isHidden()) {
            i++;
        }
    }
    return i;
}

int
pSetProgress::freeUID()
{
    static int i = 0;
    return i++;
}

void
pSetProgress::on_setProgress(int uid, int cur, int max, QString title)
{
    if (!_bars.contains(uid)) {
        _bars[uid] = new ProgressItem(max, title, _win);
        _bars[uid]->setObjectName(QString::number(uid)); // for delete from qmap

        connect(_bars[uid], SIGNAL(clicked()), this, SLOT(hideBar()));
        connect(_bars[uid], SIGNAL(completed()), this, SLOT(on_barCompleted()));

        _bars[uid]->move(NextPosition(activeBars())); // new progressitem on creation is hidden
        _bars[uid]->activate();
    }

    _bars[uid]->setTitle(title);
    _bars[uid]->setProgress(cur);
}
