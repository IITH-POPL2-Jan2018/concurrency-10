#include "gamestate.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <iostream>

GameState::GameState()
{
    //qDebug()<<bullets.size();
    //qDebug()<<bullets.size();

    bullets = std::vector<bullet*>();
    //qDebug()<<bullets.size();

    players = std::vector<player*>();

}

void GameState::addBullet(bullet *new_bullet){
    bullets.push_back(new_bullet);
}

void GameState::addPlayer(player *new_player){
    players.push_back(new_player);
}

void GameState::updatePlayer(int id, QPoint new_pos)
{
    players.at(id)->setPos(new_pos);
}

// function for creating bullet at server
bullet* GameState::createBullet(bool team1, qreal x,qreal y)
{
    bullet *new_bullet = new bullet(bullet_count,team1);
    new_bullet->setPos(x,y);
    // mutex wait(bullet_count)
    bullets.push_back(new_bullet);
    bullet_count++;
    // mutex signal(bullet_count)
    return new_bullet;
}

QJsonDocument GameState::getJsonDocFromGameState(){

    QJsonObject gamestate1;
        QJsonArray bulletArray;

            for(auto it = bullets.begin() ; it != bullets.end() ; it++)
            {
                if(*it!=NULL){
                    QJsonObject n_bullet;
                    n_bullet.insert("id",(*it)->id);
                    n_bullet.insert("posX",(*it)->pos().x());
                    n_bullet.insert("posY",(*it)->pos().y());
                    n_bullet.insert("team",(*it)->team);
                    bulletArray.push_back(n_bullet);
                }

            }

        gamestate1.insert("bullets", bulletArray);

        QJsonArray playerArray;

            for(auto it = players.begin() ; it != players.end() ; it++)
            {
                QJsonObject n_player;
                n_player.insert("id",(*it)->id);
                n_player.insert("posX",(*it)->pos().x());
                n_player.insert("posY",(*it)->pos().y());
                n_player.insert("team",(*it)->team);
                playerArray.push_back(n_player);
            }
        gamestate1.insert("players", playerArray);

        QJsonDocument doc(gamestate1);

        return doc;
}



