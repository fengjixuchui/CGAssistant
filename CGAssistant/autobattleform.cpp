#include "autobattleform.h"
#include "ui_autobattleform.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QString s_BattleCondType[BattleCond_Type_Max];
QString s_BattleCondRelationNumber[BattleCond_NumRel_Max];
QString s_BattleCondRelationString[BattleCond_StrRel_Max];
QString s_BattlePlayerActionString[BattlePlayerAction_Max];
QString s_BattleTargetString[BattleTarget_Max];
QString s_BattleTargetSelectString[BattleTarget_Select_Max];
QString s_BattlePetActionString[BattlePetAction_Max];

Q_DECLARE_METATYPE(CBattleSettingList)

AutoBattleForm::AutoBattleForm(CBattleWorker *worker, QWidget *parent) :
    QWidget(parent), m_worker(worker),
    ui(new Ui::AutoBattleForm)
{
    qRegisterMetaType<CBattleSettingList>("CBattleSettingList");

    ui->setupUi(this);

    ui->label_delay_from_val->setText(QString("%1 ms").arg(ui->horizontalSlider_delayFrom->value()));
    ui->label_delay_to_val->setText(QString("%1 ms").arg(ui->horizontalSlider_delayTo->value()));

    s_BattleCondRelationNumber[BattleCond_NumRel_EGT] = tr(">=");
    s_BattleCondRelationNumber[BattleCond_NumRel_GT] = tr(">");
    s_BattleCondRelationNumber[BattleCond_NumRel_ELT] = tr("<=");
    s_BattleCondRelationNumber[BattleCond_NumRel_LT] = tr("<");
    s_BattleCondRelationNumber[BattleCond_NumRel_EQ] = tr("=");
    s_BattleCondRelationNumber[BattleCond_NumRel_NEQ] = tr("!=");

    s_BattleCondRelationString[BattleCond_StrRel_CONTAIN] = tr("contains");
    s_BattleCondRelationString[BattleCond_StrRel_NOT_CONTAIN] = tr("not contain");

    s_BattleCondType[BattleCond_Type_EnemyCount] = tr("Enemy Count");
    s_BattleCondType[BattleCond_Type_PlayerHp] = tr("Player HP");
    s_BattleCondType[BattleCond_Type_PlayerMp] = tr("Player MP");
    s_BattleCondType[BattleCond_Type_PetHp] = tr("Pet HP");
    s_BattleCondType[BattleCond_Type_PetMp] = tr("Pet MP");
    s_BattleCondType[BattleCond_Type_TeammateHp] = tr("Teammate HP");
    s_BattleCondType[BattleCond_Type_EnemyUnit] = tr("Enemy Unit");

    s_BattlePlayerActionString[BattlePlayerAction_Ignore] = tr("Ignore");
    s_BattlePlayerActionString[BattlePlayerAction_Attack] = tr("Attack");
    s_BattlePlayerActionString[BattlePlayerAction_Defense] = tr("Defense");
    s_BattlePlayerActionString[BattlePlayerAction_Escape] = tr("Escape");
    s_BattlePlayerActionString[BattlePlayerAction_ExchangePosition] = tr("Position");
    s_BattlePlayerActionString[BattlePlayerAction_ChangePet] = tr("Pet");
    s_BattlePlayerActionString[BattlePlayerAction_UseItem] = tr("UseItem");

    s_BattleTargetString[BattleTarget_Enemy] = tr("Enemy");
    s_BattleTargetString[BattleTarget_Teammate] = tr("Teammate");
    s_BattleTargetString[BattleTarget_Self] = tr("Player");
    s_BattleTargetString[BattleTarget_Pet] = tr("Pet");

    s_BattleTargetSelectString[BattleTarget_Select_Random] = tr("Random");
    s_BattleTargetSelectString[BattleTarget_Select_Front] = tr("Front");
    s_BattleTargetSelectString[BattleTarget_Select_Back] = tr("Back");
    s_BattleTargetSelectString[BattleTarget_Select_LowHP] = tr("Low HP");
    s_BattleTargetSelectString[BattleTarget_Select_HighHP] = tr("High HP");
    s_BattleTargetSelectString[BattleTarget_Select_LowLv] = tr("Low Lv");
    s_BattleTargetSelectString[BattleTarget_Select_HighLv] = tr("High Lv");
    s_BattleTargetSelectString[BattleTarget_Select_Goatfarm] = tr("Goatfarm");
    s_BattleTargetSelectString[BattleTarget_Select_Boomerang] = tr("Boomerang");

    s_BattlePetActionString[BattlePetAction_Ignore] = tr("Ignore");

    for(int i = 0;i < BattleCond_Type_Max; ++i)
        ui->comboBox_condition_type->addItem(s_BattleCondType[i]);

    for(int i = 0;i < BattlePlayerAction_Max; ++i)
        ui->comboBox_playerAction->addItem(s_BattlePlayerActionString[i]);

    for(int i = 0;i < BattlePetAction_Max; ++i)
        ui->comboBox_petAction->addItem(s_BattlePetActionString[i]);

    for(int i = 0;i < BattleTarget_Max; ++i)
        ui->comboBox_playerTarget->addItem(s_BattleTargetString[i]);

    for(int i = 0;i < BattleTarget_Max; ++i)
        ui->comboBox_petTarget->addItem(s_BattleTargetString[i]);

    m_model = new CBattleSettingModel(ui->tableView_settings);
    ui->tableView_settings->setModel(m_model);
    ui->tableView_settings->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
    ui->tableView_settings->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    ui->tableView_settings->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    ui->tableView_settings->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    ui->tableView_settings->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeMode::Stretch);
}

AutoBattleForm::~AutoBattleForm()
{
    delete ui;
}

void AutoBattleForm::SyncAutoBattleWorker()
{
    m_worker->m_bAutoBattle = ui->checkBox_autoBattle->isChecked();
    m_worker->m_bHighSpeed = ui->checkBox_highSpeed->isChecked();
    m_worker->m_bFirstRoundNoDelay = ui->checkBox_firstRoundNoDelay->isChecked();
    m_worker->m_bLevelOneProtect = ui->checkBox_levelOneProtect->isChecked();
    m_worker->m_bLockCountdown = ui->checkBox_lockCountdown->isChecked();
    m_worker->m_iDelayFrom = ui->horizontalSlider_delayFrom->value();
    m_worker->m_iDelayTo = ui->horizontalSlider_delayTo->value();

    connect(ui->checkBox_autoBattle, SIGNAL(stateChanged(int)), m_worker, SLOT(OnSetAutoBattle(int)), Qt::ConnectionType::QueuedConnection);
    connect(ui->checkBox_highSpeed, SIGNAL(stateChanged(int)), m_worker, SLOT(OnSetHighSpeed(int)), Qt::ConnectionType::QueuedConnection);
    connect(ui->checkBox_firstRoundNoDelay, SIGNAL(stateChanged(int)), m_worker, SLOT(OnSetFRND(int)), Qt::ConnectionType::QueuedConnection);
    connect(ui->checkBox_levelOneProtect, SIGNAL(stateChanged(int)), m_worker, SLOT(OnSetLv1Protect(int)), Qt::ConnectionType::QueuedConnection);
    connect(ui->checkBox_lockCountdown, SIGNAL(stateChanged(int)), m_worker, SLOT(OnSetLockCountdown(int)), Qt::ConnectionType::QueuedConnection);
    connect(ui->checkBox_showHPMP, SIGNAL(stateChanged(int)), m_worker, SLOT(OnSetShowHPMP(int)), Qt::ConnectionType::QueuedConnection);
    connect(ui->horizontalSlider_delayFrom, SIGNAL(valueChanged(int)), m_worker, SLOT(OnSetDelayFrom(int)), Qt::ConnectionType::QueuedConnection);
    connect(ui->horizontalSlider_delayTo, SIGNAL(valueChanged(int)), m_worker, SLOT(OnSetDelayTo(int)), Qt::ConnectionType::QueuedConnection);
    connect(m_model, SIGNAL(syncList(CBattleSettingList)), m_worker, SLOT(OnSyncList(CBattleSettingList)), Qt::ConnectionType::QueuedConnection);
}

void AutoBattleForm::OnNotifyGetSkillsInfo(QSharedPointer<CGA_SkillList_t> skills)
{
    for(int i = 0;i < skills->size(); ++i)
    {
        const CGA_SkillInfo_t &skill = skills->at(i);
        if(BattlePlayerAction_Max + i < ui->comboBox_playerAction->count())
        {
            ui->comboBox_playerAction->setItemText(BattlePlayerAction_Max + i, skill.name);
            ui->comboBox_playerAction->setItemData(BattlePlayerAction_Max + i, QVariant(skill.lv));
        }
        else
        {
            ui->comboBox_playerAction->addItem(skill.name, QVariant(skill.lv));
        }
    }
    for(int i = skills->size() + BattlePlayerAction_Max; i < ui->comboBox_playerAction->count(); ++i)
    {
        ui->comboBox_playerAction->removeItem(i);
    }
}

void AutoBattleForm::OnNotifyGetPetsInfo(QSharedPointer<CGA_PetList_t> pets)
{
    for(int i = 0;i < pets->size(); ++i)
    {
        if(pets->at(i).battle_flags & 2)
        {
            const CGA_PetSkillList_t &skills = pets->at(i).skills;
            for(int j = 0; j < skills.size(); ++j)
            {
                if(BattlePetAction_Max + j < ui->comboBox_petAction->count())
                {
                    ui->comboBox_petAction->setItemText(BattlePetAction_Max + j, skills.at(j).name);
                }
                else
                {
                    ui->comboBox_petAction->addItem(skills.at(j).name);
                }
            }
            for(int j = skills.size() + BattlePetAction_Max; j < ui->comboBox_petAction->count(); ++j)
            {
                ui->comboBox_petAction->removeItem(j);
            }
            QString str = tr("from %1").arg(pets->at(i).showname);
            if(str != ui->label_petSkillsFrom->text())
                ui->label_petSkillsFrom->setText(str);

            return;
        }
    }
    for(int j = BattlePetAction_Max; j < ui->comboBox_petAction->count(); ++j)
    {
        ui->comboBox_petAction->removeItem(j);
    }
    QString str = tr("No pet");
    if(str != ui->label_petSkillsFrom->text())
        ui->label_petSkillsFrom->setText(str);

    m_PetList = pets;
}

void AutoBattleForm::OnNotifyGetItemsInfo(QSharedPointer<CGA_ItemList_t> items)
{
    m_ItemList = items;
}

void AutoBattleForm::OnCloseWindow()
{

}

void AutoBattleForm::on_horizontalSlider_delayFrom_valueChanged(int value)
{
    ui->label_delay_from_val->setText(QString("%1 ms").arg(value));
}

void AutoBattleForm::on_horizontalSlider_delayTo_valueChanged(int value)
{
    ui->label_delay_to_val->setText(QString("%1 ms").arg(value));
}

void AutoBattleForm::on_comboBox_condition_type_currentIndexChanged(int index)
{
    ui->comboBox_condition_relation->clear();
    ui->comboBox_condition_value->clear();
    switch(index)
    {
    case BattleCond_Type_EnemyCount:
    case BattleCond_Type_PlayerHp:
    case BattleCond_Type_PlayerMp:
    case BattleCond_Type_PetHp:
    case BattleCond_Type_PetMp:
    case BattleCond_Type_TeammateHp:
    {
        for(int i = 0;i < BattleCond_NumRel_Max; ++i)
            ui->comboBox_condition_relation->addItem(s_BattleCondRelationNumber[i]);

        if(index == 0)
        {
            for(int i = 1;i <= 10; ++i)
                ui->comboBox_condition_value->addItem(tr("%1").arg(i));
        } else {
            ui->comboBox_condition_value->addItem(QLatin1String("25%"));
            ui->comboBox_condition_value->addItem(QLatin1String("50%"));
            ui->comboBox_condition_value->addItem(QLatin1String("75%"));
            ui->comboBox_condition_value->addItem(QLatin1String("500"));
            ui->comboBox_condition_value->addItem(QLatin1String("1000"));
        }
        break;
    }
    case BattleCond_Type_EnemyUnit:
    {
        for(int i = 0;i < BattleCond_StrRel_Max; ++i)
            ui->comboBox_condition_relation->addItem(s_BattleCondRelationString[i]);
        break;
    }
    }
}

void AutoBattleForm::on_comboBox_playerAction_currentIndexChanged(int index)
{
    ui->comboBox_playerActionValue->clear();
    switch(index)
    {
    case BattlePlayerAction_Ignore:
    {
        //NO TARGET
        ui->comboBox_playerTarget->setEnabled(false);
        ui->comboBox_playerTargetSelect->setEnabled(false);
        ui->comboBox_playerActionValue->setEnabled(false);
        break;
    }
    case BattlePlayerAction_Attack:
    {
        ui->comboBox_playerTarget->setEnabled(true);
        ui->comboBox_playerTargetSelect->setEnabled(true);
        ui->comboBox_playerActionValue->setEnabled(false);
        break;
    }
    case BattlePlayerAction_Defense:
    {
        //NO TARGET
        ui->comboBox_playerTarget->setEnabled(false);
        ui->comboBox_playerTargetSelect->setEnabled(false);
        ui->comboBox_playerActionValue->setEnabled(false);
        break;
    }
    case BattlePlayerAction_Escape:
    {
        ui->comboBox_playerTarget->setEnabled(false);
        ui->comboBox_playerTargetSelect->setEnabled(false);
        ui->comboBox_playerActionValue->setEnabled(false);
        break;
    }
    case BattlePlayerAction_ExchangePosition:
    {
        ui->comboBox_playerTarget->setEnabled(false);
        ui->comboBox_playerTargetSelect->setEnabled(false);
        ui->comboBox_playerActionValue->setEnabled(false);
        break;
    }
    case BattlePlayerAction_ChangePet:
    {
        //TODO:Fill item list here...
        ui->comboBox_playerTarget->setEnabled(false);
        ui->comboBox_playerTargetSelect->setEnabled(false);
        ui->comboBox_playerActionValue->setEnabled(true);

        ui->comboBox_playerActionValue->addItem(tr("Recall"), QVariant(1));
        ui->comboBox_playerActionValue->addItem(tr("Pet with highest lv"), QVariant(2));
        ui->comboBox_playerActionValue->addItem(tr("Pet with highest hp"), QVariant(3));
        if(m_PetList.data())
        {
            for(int i = 0;i < m_PetList->size(); ++i)
                ui->comboBox_playerActionValue->addItem(m_PetList->at(i).showname, QVariant(0));
        }
        break;
    }
    case BattlePlayerAction_UseItem:
    {
        //TODO:Fill item list here...
        ui->comboBox_playerTarget->setEnabled(true);
        ui->comboBox_playerTargetSelect->setEnabled(true);
        ui->comboBox_playerActionValue->setEnabled(true);

        if(m_ItemList.data())
        {
            for(int i = 0;i < m_ItemList->size(); ++i)
                ui->comboBox_playerActionValue->addItem(m_ItemList->at(i).name, QVariant(m_ItemList->at(i).itemid));
        }
        break;
    }
    default://Custom skills...
    {
        ui->comboBox_playerTarget->setEnabled(true);
        ui->comboBox_playerTargetSelect->setEnabled(true);
        ui->comboBox_playerActionValue->setEnabled(true);
        bool bValue = false;
        int level = ui->comboBox_playerAction->itemData(index).toInt(&bValue);
        if(bValue && level > 0){
            ui->comboBox_playerActionValue->addItem(tr("Lv max"), QVariant(0));
            for(int i = 1; i <= level; ++i)
                ui->comboBox_playerActionValue->addItem(tr("Lv %1").arg(i), QVariant(i));
        }
        break;
    }
    }
}

void AutoBattleForm::on_comboBox_petAction_currentIndexChanged(int index)
{
    switch(index)
    {
    case BattlePetAction_Ignore:
    {
        ui->comboBox_petTarget->setEnabled(false);
        ui->comboBox_petTargetSelect->setEnabled(false);
        break;
    }
    default://Custom skills...
    {
        ui->comboBox_petTarget->setEnabled(true);
        ui->comboBox_petTargetSelect->setEnabled(true);
        break;
    }
    }
}

void AutoBattleForm::on_comboBox_playerTarget_currentIndexChanged(int index)
{
    ui->comboBox_playerTargetSelect->clear();
    switch(index)
    {
    case BattleTarget_Enemy:
    {
        for(int i = 0;i < BattleTarget_Select_Max; ++i)
            ui->comboBox_playerTargetSelect->addItem(s_BattleTargetSelectString[i]);

        break;
    }
    case BattleTarget_Teammate:
    {
        for(int i = 0;i < BattleTarget_Select_Goatfarm; ++i)
            ui->comboBox_playerTargetSelect->addItem(s_BattleTargetSelectString[i]);
        break;
    }
    case BattleTarget_Self:
    {
        break;
    }
    case BattleTarget_Pet:
    {
        break;
    }
    }
}

void AutoBattleForm::on_comboBox_petTarget_currentIndexChanged(int index)
{
    ui->comboBox_petTargetSelect->clear();
    switch(index)
    {
    case BattleTarget_Enemy:
    {
        for(int i = 0;i < BattleTarget_Select_Max; ++i)
            ui->comboBox_petTargetSelect->addItem(s_BattleTargetSelectString[i]);

        break;
    }
    case BattleTarget_Teammate:
    {
        for(int i = 0;i < BattleTarget_Select_Goatfarm; ++i)
            ui->comboBox_petTargetSelect->addItem(s_BattleTargetSelectString[i]);
        break;
    }
    case BattleTarget_Self:
    {
        break;
    }
    case BattleTarget_Pet:
    {
        break;
    }
    }
}


void AutoBattleForm::on_pushButton_add_clicked()
{
    int condType = ui->comboBox_condition_type->currentIndex();
    CBattleCondition *pCondition = NULL;
    switch(condType)
    {
    case BattleCond_Type_EnemyCount:
    {
        int relation = ui->comboBox_condition_relation->currentIndex();
        if(relation >= 0 && relation < BattleCond_NumRel_Max)
        {
            bool bValue = false;
            int value = ui->comboBox_condition_value->currentText().toInt(&bValue);
            if(bValue && value >= 0 && value <= 10)
                pCondition = new CBattleCondition_EnemyCount(relation, value);
        }
        break;
    }
    case BattleCond_Type_PlayerHp:
    case BattleCond_Type_PlayerMp:
    case BattleCond_Type_PetHp:
    case BattleCond_Type_PetMp:
    case BattleCond_Type_TeammateHp:
    {
        int relation = ui->comboBox_condition_relation->currentIndex();
        if(relation >= 0 && relation < BattleCond_NumRel_Max)
        {
            QString str = ui->comboBox_condition_value->currentText();
            int per = str.indexOf(QChar('%'));
            bool bValue = false, bPercentage = false;
            int value = 0;
            if(per >= 1)
            {
                value = str.mid(0, per).toInt(&bValue);
                if(bValue && value >= 0 && value <= 100)
                    bPercentage = true;
                else
                    bValue  = false;
            } else {
                value = str.toInt(&bValue);
                if(bValue && value >= 0)
                    bPercentage = false;
                else
                    bValue  = false;
            }
            if(bValue)
            {
                if(condType == BattleCond_Type_PlayerHp)
                    pCondition = new CBattleCondition_PlayerHp(relation, value, bPercentage);
                else if(condType == BattleCond_Type_PlayerMp)
                    pCondition = new CBattleCondition_PlayerMp(relation, value, bPercentage);
                else if(condType == BattleCond_Type_PetHp)
                    pCondition = new CBattleCondition_PetHp(relation, value, bPercentage);
                else if(condType == BattleCond_Type_PetMp)
                    pCondition = new CBattleCondition_PetMp(relation, value, bPercentage);
                else if(condType == BattleCond_Type_TeammateHp)
                    pCondition = new CBattleCondition_TeammateHp(relation, value, bPercentage);
            }
        }
        break;
    }
    case BattleCond_Type_EnemyUnit:
    {
        int relation = ui->comboBox_condition_relation->currentIndex();
        if(relation >= 0 && relation < BattleCond_StrRel_Max)
        {
            QString name = ui->comboBox_condition_value->currentText();
            if(!name.isEmpty())
                pCondition = new CBattleCondition_EnemyUnit(relation, name);

        }
        break;
    }
    }

    CBattleAction *pPlayerAction = NULL;

    int playerActionIndex = ui->comboBox_playerAction->currentIndex();
    QString playerActionStr = ui->comboBox_playerAction->currentText();

    if(ui->comboBox_playerAction->itemText(playerActionIndex) != playerActionStr)
        playerActionIndex = -1;

    bool bHasPlayerTarget = ui->comboBox_playerTarget->isEnabled();
    switch(playerActionIndex)
    {
    case BattlePlayerAction_Ignore:
    {
        pPlayerAction = NULL;
        break;
    }
    case BattlePlayerAction_Attack:
    {
        pPlayerAction = new CBattleAction_PlayerAttack();
        break;
    }
    case BattlePlayerAction_Defense:
    {
        pPlayerAction = new CBattleAction_PlayerDefense();
        break;
    }
    case BattlePlayerAction_Escape:
    {
        pPlayerAction = new CBattleAction_PlayerEscape();
        break;
    }
    case BattlePlayerAction_ExchangePosition:
    {
        pPlayerAction = new CBattleAction_PlayerExchangePosition();
        break;
    }
    case BattlePlayerAction_ChangePet:
    {
        bool bValue = false;
        int type = ui->comboBox_playerActionValue->currentData().toInt(&bValue);
        if(bValue && type > 0)
        {
            pPlayerAction = new CBattleAction_PlayerChangePet(type);
        }
        else
        {
            QString petName = ui->comboBox_playerActionValue->currentText();
            if(!petName.isEmpty())
                pPlayerAction = new CBattleAction_PlayerChangePet(petName);
        }
        break;
    }
    case BattlePlayerAction_UseItem:
    {
        bool bValue = false;
        int itemId = ui->comboBox_playerActionValue->currentData().toInt(&bValue);
        if(bValue)
        {
            pPlayerAction = new CBattleAction_PlayerUseItem(itemId);
        }
        else
        {
            QString itemName = ui->comboBox_playerActionValue->currentText();
            if(!itemName.isEmpty())
                pPlayerAction = new CBattleAction_PlayerUseItem(itemName);
        }
        break;
    }
    default:
    {
        if(!playerActionStr.isEmpty())
        {
            bool bValue = false;
            int skillLevel = ui->comboBox_playerActionValue->currentData().toInt(&bValue);
            if(bValue && skillLevel >= 0)
                pPlayerAction = new CBattleAction_PlayerSkillAttack(playerActionStr, skillLevel);
        }
        break;
    }
    }

    CBattleAction *pPetAction = NULL;

    int petActionIndex = ui->comboBox_petAction->currentIndex();
    QString petActionStr = ui->comboBox_petAction->currentText();
    bool bHasPetTarget = ui->comboBox_petTarget->isEnabled();

    if(ui->comboBox_petAction->itemText(petActionIndex) != petActionStr)
        petActionIndex = -1;

    switch(petActionIndex)
    {
    case BattlePetAction_Ignore:
    {
        pPetAction = NULL;
        break;
    }
    default:
    {
        if(!petActionStr.isEmpty())
        {
            pPetAction = new CBattleAction_PetSkillAttack(petActionStr);
        }
        break;
    }
    }

    CBattleTarget *pPlayerTarget = NULL;
    if(bHasPlayerTarget)
    {
        int playerTargetIndex = ui->comboBox_playerTarget->currentIndex();
        switch(playerTargetIndex)
        {
        case BattleTarget_Enemy:
        {
            int playerTargetSelectIndex = ui->comboBox_playerTargetSelect->currentIndex();
            if(playerTargetSelectIndex >= 0 && playerTargetSelectIndex < BattleTarget_Select_Max)
                pPlayerTarget = new CBattleTarget_Enemy(playerTargetSelectIndex);
            break;
        }
        case BattleTarget_Teammate:
        {
            int playerTargetSelectIndex = ui->comboBox_playerTargetSelect->currentIndex();
            if(playerTargetSelectIndex >= 0 && playerTargetSelectIndex < BattleTarget_Select_Goatfarm)
                pPlayerTarget = new CBattleTarget_Teammate(playerTargetSelectIndex);
            break;
        }
        case BattleTarget_Self:
        {
            pPlayerTarget = new CBattleTarget_Self();
            break;
        }
        case BattleTarget_Pet:
        {
            pPlayerTarget = new CBattleTarget_Pet();
            break;
        }
        }
    }

    CBattleTarget *pPetTarget = NULL;
    if(bHasPetTarget)
    {
        int petTargetIndex = ui->comboBox_petTarget->currentIndex();
        switch(petTargetIndex)
        {
        case BattleTarget_Enemy:
        {
            int petTargetSelectIndex = ui->comboBox_petTargetSelect->currentIndex();
            if(petTargetSelectIndex >= 0 && petTargetSelectIndex < BattleTarget_Select_Max)
                pPetTarget = new CBattleTarget_Enemy(petTargetSelectIndex);
            break;
        }
        case BattleTarget_Teammate:
        {
            int petTargetSelectIndex = ui->comboBox_petTargetSelect->currentIndex();
            if(petTargetSelectIndex >= 0 && petTargetSelectIndex < BattleTarget_Select_Goatfarm)
                pPetTarget = new CBattleTarget_Teammate(petTargetSelectIndex);
            break;
        }
        case BattleTarget_Self:
        {
            pPetTarget = new CBattleTarget_Self();
            break;
        }
        case BattleTarget_Pet:
        {
            pPetTarget = new CBattleTarget_Pet();
            break;
        }
        }
    }
    CBattleSettingPtr ptr(new CBattleSetting(pCondition, pPlayerAction, pPlayerTarget, pPetAction, pPetTarget));
    if( pCondition && ( pPlayerAction || pPetAction ) )
    {
        m_model->appendRow(ptr);
        ui->tableView_settings->resizeRowToContents(m_model->rowCount() - 1);
    }
}

void AutoBattleForm::on_pushButton_del_clicked()
{
    if(ui->tableView_settings->currentIndex().isValid())
    {
        m_model->removeRow(ui->tableView_settings->currentIndex().row());
    }
}

void AutoBattleForm::on_pushButton_moveUp_clicked()
{
    if(ui->tableView_settings->currentIndex().isValid())
    {
        int row = ui->tableView_settings->currentIndex().row();
        if(row > 0)
        {
            m_model->swapRow(row, row - 1);
            ui->tableView_settings->selectRow(row - 1);
        }
    }
}

void AutoBattleForm::on_pushButton_moveDown_clicked()
{
    if(ui->tableView_settings->currentIndex().isValid())
    {
        int row = ui->tableView_settings->currentIndex().row();
        if(row < m_model->rowCount() - 1)
        {
            m_model->swapRow(row, row + 1);
            ui->tableView_settings->selectRow(row + 1);
        }
    }
}

bool AutoBattleForm::ParseBattleSettings(const QJsonValue &val)
{
    if(!val.isObject())
        return false;

    auto obj = val.toObject();

    ui->checkBox_showHPMP->setChecked(obj.take("showhpmp").toBool());
    ui->checkBox_highSpeed->setChecked(obj.take("highspeed").toBool());
    ui->checkBox_autoBattle->setChecked(obj.take("autobattle").toBool());
    ui->checkBox_lockCountdown->setChecked(obj.take("lockcd").toBool());
    ui->checkBox_levelOneProtect->setChecked(obj.take("lv1prot").toBool());
    ui->checkBox_firstRoundNoDelay->setChecked(obj.take("r1nodelay").toBool());
    ui->horizontalSlider_delayFrom->setValue(obj.take("delayfrom").toInt());
    ui->horizontalSlider_delayTo->setValue(obj.take("delayto").toInt());

    m_model->removeRows(0, m_model->rowCount());

    auto list = obj.take("list");
    if(list.isArray()){
        auto arr = list.toArray();
        for (auto i = 0;i < arr.size(); ++i){
            auto setting = arr[i].toObject();
            auto conditionTypeId = setting.take("condition").toInt();
            auto conditionSelId = setting.take("conditionsel").toInt();
            auto conditionValue = setting.take("conditionval").toString();
            ui->comboBox_condition_type->setCurrentIndex(conditionTypeId);
            on_comboBox_condition_type_currentIndexChanged(ui->comboBox_condition_type->currentIndex());
            ui->comboBox_condition_relation->setCurrentIndex(conditionSelId);
            ui->comboBox_condition_value->setCurrentText(conditionValue);

            auto playerActionId = setting.take("playeraction").toInt();

            if(playerActionId == BattlePlayerAction_ChangePet || playerActionId == BattlePlayerAction_UseItem){
                ui->comboBox_playerAction->setCurrentIndex(playerActionId);
                on_comboBox_playerAction_currentIndexChanged(ui->comboBox_playerAction->currentIndex());
                auto playerActionValue = setting.take("playeractionval").toString();
                ui->comboBox_playerActionValue->setCurrentText(playerActionValue);
            } else if(playerActionId == BattlePlayerAction_Skill) {
                auto playerSkillName = setting.take("playerskillname").toString();
                auto playerSkillLevel = setting.take("playerskilllevel").toInt();
                ui->comboBox_playerAction->setCurrentText(playerSkillName);
                ui->comboBox_playerActionValue->clear();
                ui->comboBox_playerActionValue->addItem(tr("Lv Max"), QVariant(0));
                for(auto lv = 1; lv <= 10; ++lv)
                    ui->comboBox_playerActionValue->addItem(tr("Lv %1").arg(lv), QVariant(lv));
                ui->comboBox_playerActionValue->setCurrentIndex(playerSkillLevel);
                ui->comboBox_playerTarget->setEnabled(true);
            } else {
                ui->comboBox_playerAction->setCurrentIndex(playerActionId);
                on_comboBox_playerAction_currentIndexChanged(ui->comboBox_playerAction->currentIndex());
            }

            auto playerTargetId = setting.take("playertarget").toInt();
            auto playerTargetSel = setting.take("playertargetsel").toInt();
            ui->comboBox_playerTarget->setCurrentIndex(playerTargetId);
            on_comboBox_playerTarget_currentIndexChanged(ui->comboBox_playerTarget->currentIndex());
            ui->comboBox_playerTargetSelect->setCurrentIndex(playerTargetSel);

            auto petActionId = setting.take("petaction").toInt();

            if(petActionId == BattlePetAction_Skill){
                auto petSkillName = setting.take("petskillname").toString();
                ui->comboBox_petAction->setCurrentText(petSkillName);
                on_comboBox_petAction_currentIndexChanged(BattlePetAction_Skill);
            }
            else
            {
                ui->comboBox_petAction->setCurrentIndex(petActionId);
                on_comboBox_petAction_currentIndexChanged(ui->comboBox_petAction->currentIndex());
            }

            auto petTargetId = setting.take("pettarget").toInt();
            auto petTargetSel = setting.take("pettargetsel").toInt();
            ui->comboBox_petTarget->setCurrentIndex(petTargetId);
            on_comboBox_petTarget_currentIndexChanged(ui->comboBox_petTarget->currentIndex());
            ui->comboBox_petTargetSelect->setCurrentIndex(petTargetSel);
            ui->pushButton_add->click();
        }
    }

    SyncAutoBattleWorker();
    return true;
}

void AutoBattleForm::SaveBattleSettings(QJsonObject &obj)
{
    obj.insert("showhpmp", ui->checkBox_showHPMP->isChecked());
    obj.insert("highspeed", ui->checkBox_highSpeed->isChecked());
    obj.insert("autobattle", ui->checkBox_autoBattle->isChecked());
    obj.insert("lockcd", ui->checkBox_lockCountdown->isChecked());
    obj.insert("lv1prot", ui->checkBox_levelOneProtect->isChecked());
    obj.insert("r1nodelay", ui->checkBox_firstRoundNoDelay->isChecked());
    obj.insert("delayfrom", ui->horizontalSlider_delayFrom->value());
    obj.insert("delayto", ui->horizontalSlider_delayTo->value());

    QJsonArray list;
    for(auto i = 0;i < m_model->rowCount(); ++i)
    {
        QJsonObject row;
        auto setting = m_model->BattleSettingFromIndex(m_model->index(i, 0));
        row.insert("index", i);
        row.insert("condition", setting->GetConditionTypeId());
        row.insert("conditionrel", setting->GetConditionRelId());
        QString conditionVauleStr;
        setting->GetConditionValue(conditionVauleStr);
        row.insert("conditionval", conditionVauleStr);
        row.insert("playeraction", setting->GetPlayerActionTypeId());
        if(setting->GetPlayerActionTypeId() == BattlePlayerAction_ChangePet
                || setting->GetPlayerActionTypeId() == BattlePlayerAction_UseItem){
            QString actionVauleStr;
            setting->GetPlayerActionName(actionVauleStr);
            row.insert("playeractionval", actionVauleStr);
        } else if(setting->GetPlayerActionTypeId() == BattlePlayerAction_Skill){
            row.insert("playerskillname", setting->GetPlayerSkillName());
            row.insert("playerskilllevel", setting->GetPlayerSkillLevel());
        }
        row.insert("playertarget", setting->GetPlayerTargetTypeId());
        row.insert("playertargetsel", setting->GetPlayerTargetSelectId());
        row.insert("petaction", setting->GetPetActionTypeId());
        if(setting->GetPetActionTypeId() == BattlePetAction_Skill){
            row.insert("petskillname", setting->GetPetSkillName());
        }
        row.insert("pettarget", setting->GetPetTargetTypeId());
        row.insert("pettargetsel", setting->GetPetTargetSelectId());
        list.insert(i, row);
    }

    obj.insert("list", list);
}
