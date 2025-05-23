#include "../include/autonomy.h"

//constructor
Autonomy::Autonomy(QWidget *parent)
    : QWidget{parent}
{
    if (parent)
    {
        setMinimumSize(parent->width() * 0.2, parent->height() * 0.16); 
        setMaximumSize(parent->width() * 0.2, parent->height() * 0.16);
    }
    main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(height() * 0.05);
    layout = new QHBoxLayout();
    layout->setSpacing(width() * 0.0155); 
    nb_sections = 6;
    for (int i = 0; i < nb_sections; ++i)
    {
        QWidget *section = new QWidget(this);
        section->setFixedHeight(height() * 0.3);
        section->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        layout->addWidget(section);
        sections.append(section);
    }
    main_layout->addLayout(layout);
    label = new QLabel(this);
    set_autonomy(8);
}

//destructor
Autonomy::~Autonomy()
{
    std::cout << "Remove Autonomy" << std::endl;
    delete layout;
}

//setter
void Autonomy::set_autonomy(int aut)
{
    int sections_color = static_cast<int>((aut / 10.0) * nb_sections);
    for (int i = nb_sections -1; i >= 0; i--)
    {
        if (i >= nb_sections - sections_color)
        {
            QColor section_color;
            if (aut > 6)
            {
                int red = std::max(0, 120 - ((nb_sections - 1 - i) * (255 / nb_sections))); 
                int green = std::min(255, 80 + ((nb_sections - 1 - i) * (20 / nb_sections))); 
                int blue = std::min(255, 50 + ((nb_sections - 1 - i) * (50 / nb_sections)));  
                section_color.setRgb(red, green, blue);
            } else
            {
                int red = std::max(0, 120 - ((nb_sections - 1 - i) * (255 / nb_sections)));         
                int green = std::min(255, 80 + ((nb_sections - 1 - i) * (20 / nb_sections))); 
                int blue = std::min(255, 50 + ((nb_sections - 1 - i) * (50 / nb_sections)));
                section_color.setRgb(red, green, blue);
            } 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(section_color.name()));
        } else
        {
            QColor inactive_color(22, 32, 60); 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactive_color.name()));
        }
    }
    label->setTextFormat(Qt::RichText); // Enable rich text
    label->setText("<span style='font-family: Digital-7; font-size: 27px;'>" + QString::number(aut) + 
        "</span><span style='font-family: Calculator; font-size: 27px;'> km</span>");
    label->setStyleSheet("color: rgb(0, 120, 140);");
    label->setAlignment(Qt::AlignTop | Qt::AlignRight);
    label->setContentsMargins(0, 0, 5, 0);
    main_layout->addWidget(label);
}

// getters
int Autonomy::get_nbsections()
{
    return nb_sections;
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