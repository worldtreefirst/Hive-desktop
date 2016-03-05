#include "GuiSettingsStack_profile.h"

GuiSettingsStack_profile::GuiSettingsStack_profile(QWidget *parent)
{
  avatar_map.insert("Bee", ":/img/img/icon.png");
  avatar_map.insert("Fat Bee", ":/avatar/avatar/bee.png");
  avatar_map.insert("Lady Bug", ":/avatar/avatar/ladybug.png");
  avatar_map.insert("Disk", ":/avatar/avatar/disk.png");
  avatar_map.insert("Sunflower", ":/avatar/avatar/sunflower.png");
  avatar_map.insert("Worm", ":/avatar/avatar/worm.png");
  avatar_map.insert("Default", ":/avatar/avatar/default.png");

  setIcon(":/img/img/profile.png");
  setTitle("Settings");
  setSubTitle("profile");
  setTag("User information");

  //avatar
  avatar_btn = new GuiAvatarButton(GlobalData::g_avatarPathStr, 100, this);
  QButtonGroup *avatar_option_group = new QButtonGroup(this);
  QVBoxLayout *avatar_option_layout = new QVBoxLayout();
  avatar_option_layout->addWidget(avatar_btn);

  foreach(QString temp_avatar_name_str, avatar_map.keys())
    {
      QRadioButton *temp_radio_btn = new QRadioButton(temp_avatar_name_str, this);
      avatar_option_group->addButton(temp_radio_btn);
      avatar_option_layout->addWidget(temp_radio_btn);
      avatar_radio_btn_list.append(temp_radio_btn);
    }

  addItem("\tAvatar:", avatar_option_layout);

  //usrname
  QLineEdit *usr_name_line_edit = new QLineEdit(GlobalData::g_myNameStr,this);
  usr_name_line_edit->setMaximumWidth(200);
  addItem("\tUser Name:", usr_name_line_edit);

  //usrkey
  QLineEdit *usr_key_line_edit = new QLineEdit(GlobalData::g_myNameStr,this);
  usr_key_line_edit->setText(GlobalData::g_myKeyStr);
  usr_key_line_edit->setCursor(QCursor(Qt::IBeamCursor));
  usr_key_line_edit->setReadOnly(true);
  usr_key_line_edit->setMaximumWidth(200);
  addItem("\tUser Key:", usr_key_line_edit);


  connect(avatar_option_group, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onRadioClicked(QAbstractButton*)));
  connect(usr_name_line_edit, SIGNAL(textEdited(QString)), this, SLOT(onUsrNameChanged(QString)));

  this->setParent(parent);
}

GuiSettingsStack_profile::~GuiSettingsStack_profile()
{
  foreach (QRadioButton *object, avatar_radio_btn_list)
    {
      object->deleteLater();
    }
}

void GuiSettingsStack_profile::onRadioClicked(QAbstractButton *abstractButton)
{
  GlobalData::g_avatarPathStr = avatar_map.value(abstractButton->text());
  avatar_btn->setAvatar(GlobalData::g_avatarPathStr);
}

void GuiSettingsStack_profile::onUsrNameChanged(QString usr_name)
{
  GlobalData::g_myNameStr = usr_name;
}
