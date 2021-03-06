#ifndef GUIHOMESTACK_LIST_H
#define GUIHOMESTACK_LIST_H

#include "Gui/Frameworks/ScrollStack.h"
#include "GlobalData.h"

class HomeStack_list;
class ListItem;


class ListItem : public QWidget
{
  Q_OBJECT
public:
  explicit ListItem(UsrProfileStruct *usrProfileStruct, QWidget *parent = 0);
  ~ListItem();

  void refreshUsrProfile(UsrProfileStruct *usrProfileStruct);

private:
  AvatarButton *avatar_button;
  QLabel *name_label;
  QLabel *ip_label;

};


class HomeStack_list : public ScrollStack
{
  Q_OBJECT

public:
  explicit HomeStack_list(QWidget *parent);
  ~HomeStack_list();

  void addUsr(UsrProfileStruct *usrProfileStruct);
  void refreshUsrProfile(UsrProfileStruct *usrProfileStruct);

private:
  QHash<QString, ListItem*> list_item_hash;
};




#endif // GUIHOMESTACK_LIST_H

