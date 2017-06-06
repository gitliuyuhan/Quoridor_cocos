//
//  LoginLayer.cpp
//  Quoridor
//
//  Created by shiyi on 2016/11/28.
//
//

#include "LoginLayer.h"
#include "GameWithCmpLayer.h"
#include "ChoosePlayLayer.h"

#include "singleton.h"
#include "user.h"

Scene* LoginLayer::createScene()
{
    Layer* layer = LoginLayer::create();
    Scene* scene = Scene::create();
    scene->addChild(layer);
    
    return scene;
}

bool LoginLayer::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    
    
    
    auto rootNode = CSLoader::createNode("loginLayer.csb");
    auto bgSprite = (Sprite*)rootNode->getChildByName("bgSprite");
    bgSprite->removeFromParent();
    bgSprite->setPosition(ORIGIN.x + VISIBLESIZE.width/2, ORIGIN.y + VISIBLESIZE.height/2);
    addChild(bgSprite, 1);
    
    auto bg = bgSprite->getChildByName("bg");
    _user = (TextField*)bg->getChildByTag(1);
    _pass = (TextField*)bg->getChildByTag(2);
    _buttonUser = (Button*)bg->getChildByTag(3);
    _buttonPass = (Button*)bg->getChildByTag(4);
    _keepMind = (CheckBox*)bg->getChildByTag(5);
    _autoLogin = (CheckBox*)bg->getChildByTag(6);
    
    _user->addEventListener(CC_CALLBACK_2(LoginLayer::textFieldEvent, this));
    _pass->addEventListener(CC_CALLBACK_2(LoginLayer::textFieldEvent, this));
    ((Button*)bg->getChildByTag(7))->addTouchEventListener(CC_CALLBACK_2(LoginLayer::touchButton, this));
    ((Button*)bg->getChildByTag(8))->addTouchEventListener(CC_CALLBACK_2(LoginLayer::touchButton, this));
    return true;
}

void LoginLayer::textFieldEvent(Ref* ref, TextField::EventType event)
{
    int tag = ((Widget*)ref)->getTag();

    switch (event) {
        case cocos2d::ui::TextField::EventType::INSERT_TEXT:
            break;
        case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
            break;
        case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
            if(tag == 1)
            {
                _buttonUser->setBright(false);
            }
            else
            {
                _buttonPass->setBright(false);
            }
            break;
        case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
            _buttonUser->setBright(true);
            _buttonPass->setBright(true);
            break;
        default:
            break;
    }
}

void LoginLayer::touchButton(cocos2d::Ref *object, ui::Widget::TouchEventType touchType)
{
    if(touchType == Widget::TouchEventType::ENDED)
    {
        int tag = ((Node*)object)->getTag();
        switch (tag) {
                //单机
            case 7:
                startGameWhithCmp();
                break;
                //登陆
            case 8:
                doLogin();
                break;
            default:
                break;
        }
    }
}

void LoginLayer::doLogin()
{
    log("login...");
    std::string name = _user->getString();
    std::cout << name << std::endl;
    std::string pass = _pass->getString();
    std::cout << pass << std::endl;
    bool keep = _keepMind->isSelected();
    bool autoLogin = _autoLogin->isSelected();

    //登录验证
    int ret = USER->connectServer();
    if(!ret)
      return;
    ret = USER->login(name,pass);
    if(!ret)
      return;
    
    //跳转主界面
    auto scene = ChoosePlayLayer::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

void LoginLayer::touchCheck(Ref* object, CheckBox::EventType type)
{
    
}

void LoginLayer::startGameWhithCmp()
{
    log("play with computer...");
    auto scene = ChoosePlayLayer::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}
