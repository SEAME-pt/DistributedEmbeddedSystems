#include "../include/autonomy.h"

//constructor
Autonomy::Autonomy(QWidget *parent)
    : QWidget{parent}
{
    label = new QLabel(this);
    main_layout = new QVBoxLayout(this);
    layout = new QHBoxLayout();

    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(140);
    main_layout->setSpacing(10); 
    main_layout->setContentsMargins(0, 0, 0, 0); // Remove margins

    layout->setSpacing(2);
    layout->setContentsMargins(0, 0, 0, 0);

    nb_sections = 6;
    for (int i = 0; i < nb_sections; ++i)
    {
        QWidget *section = new QWidget(this);
        section->setFixedSize(25, 35);
        layout->addWidget(section);
        sections.append(section);
    }

    main_layout->addLayout(layout);
    set_autonomy(7);
    main_layout->addWidget(label); // Add label to the layout
}

//destructor
Autonomy::~Autonomy()
{
    std::cout << "Remove Autonomy" << std::endl;
}

//setter
void Autonomy::set_autonomy(int aut)
{
    autonomy = aut;
    int sections_color = static_cast<int>((aut / 10.0) * nb_sections);
    for (int i = nb_sections -1; i >= 0; i--)
    {
        if (i >= nb_sections - sections_color)
        {
            QColor startColor(0, 80, 60); // dark cyan-green
            QColor endColor;

            if (aut > 6)
            {
                // Smooth cyan-green gradient
                endColor = QColor(0, 100, 80); // slightly brighter cyan-green
            }
            else
            {
                // Gradually shift from cyan-green to reddish
                endColor = QColor(90, 15, 0); // reddish, not too bright
            }

            // Interpolate based on **active section index** (0 = leftmost active)
            int activeIndex = i - (nb_sections - sections_color);  // 0..sections_active-1
            float t = (sections_color > 1) ? float(activeIndex) / (sections_color - 1) : 0.0f;   // 0..1

            int red   = startColor.red()   + t * (endColor.red()   - startColor.red());
            int green = startColor.green() + t * (endColor.green() - startColor.green());
            int blue  = startColor.blue()  + t * (endColor.blue()  - startColor.blue());

            QColor section_color(red, green, blue);  // correct: QColor object
            sections[i]->setStyleSheet(QString("background-color: %1").arg(section_color.name()));


        } else
        {
            QColor inactive_color(22, 32, 60); 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactive_color.name()));
        }
    }
    label->setTextFormat(Qt::RichText); // Enable rich text
    label->setText("<span style='font-family: Digital-7; font-size: 30px;'>" + QString::number(aut) + 
        "</span><span style='font-family: Calculator; font-size: 32px;'> km</span>");
    label->setStyleSheet("color: rgb(0, 120, 140);");
    label->setAlignment(Qt::AlignRight);
    label->setContentsMargins(0, 0, 5, 0);
}

// getters
int Autonomy::get_nbsections()
{
    return nb_sections;
}

int Autonomy::get_autonomy()
{
    return autonomy;
}

QVector<QWidget*> Autonomy::get_sections()
{
    return sections;
}

QHBoxLayout* Autonomy::get_layout()
{
    return layout;
}

QVBoxLayout* Autonomy::get_mainlayout()
{
    return main_layout;
}

QLabel* Autonomy::get_label()
{
    return label;
}