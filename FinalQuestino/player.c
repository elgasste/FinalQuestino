#include "player.h"

void Player_Init( Player_t* player )
{
   Player_LoadSprite( player );

   player->position.x = 0;
   player->position.y = 0;
   player->velocity.x = 0;
   player->velocity.y = 0;
   player->maxVelocity = PLAYERVELOCITY_NORMAL;

   player->stats.hitPoints = 12;
   player->stats.maxHitPoints = 12;
   player->stats.magicPoints = 0;
   player->stats.maxMagicPoints = 0;
   player->stats.attackPower = 2;
   player->stats.defensePower = 2;
   player->stats.agility = 2;

   player->experience = 0;
   player->gold = 0;
   player->items = 0;
}

uint8_t Player_GetLevel( Player_t* player )
{
   if ( player->experience < 7 ) { return 1; }
   else if ( player->experience < 23 ) { return 2; }
   else if ( player->experience < 47 ) { return 3; }
   else if ( player->experience < 110 ) { return 4; }
   else if ( player->experience < 220 ) { return 5; }
   else if ( player->experience < 450 ) { return 6; }
   else if ( player->experience < 800 ) { return 7; }
   else if ( player->experience < 1300 ) { return 8; }
   else if ( player->experience < 2000 ) { return 9; }
   else if ( player->experience < 2900 ) { return 10; }
   else if ( player->experience < 4000 ) { return 11; }
   else if ( player->experience < 5500 ) { return 12; }
   else if ( player->experience < 7500 ) { return 13; }
   else if ( player->experience < 10000 ) { return 14; }
   else if ( player->experience < 13000 ) { return 15; }
   else if ( player->experience < 16000 ) { return 16; }
   else if ( player->experience < 19000 ) { return 17; }
   else if ( player->experience < 22000 ) { return 18; }
   else if ( player->experience < 26000 ) { return 19; }
   else if ( player->experience < 30000 ) { return 20; }
   else if ( player->experience < 34000 ) { return 21; }
   else if ( player->experience < 38000 ) { return 22; }
   else if ( player->experience < 42000 ) { return 23; }
   else if ( player->experience < 46000 ) { return 24; }
   else if ( player->experience < 50000 ) { return 25; }
   else if ( player->experience < 54000 ) { return 26; }
   else if ( player->experience < 58000 ) { return 27; }
   else if ( player->experience < 62000 ) { return 28; }
   else if ( player->experience < 65535 ) { return 29; }
   else { return 30; }
}

uint16_t Player_CollectGold( Player_t* player, uint16_t gold )
{
   if ( gold < ( UINT16_MAX - player->gold ) )
   {
      player->gold += gold;
      return gold;
   }
   else
   {
      player->gold = UINT16_MAX;
      return UINT16_MAX - player->gold;
   }
}

uint16_t Player_CollectExperience( Player_t* player, uint16_t experience )
{
   if ( experience < ( UINT16_MAX - player->experience ) )
   {
      player->experience += experience;
      return experience;
   }
   else
   {
      player->experience = UINT16_MAX;
      return UINT16_MAX - player->experience;
   }
}

Bool_t Player_CollectItem( Player_t* player, uint8_t item )
{
   Bool_t collected = False;
   int8_t count;

   switch ( item )
   {
      case ITEM_KEY:
         count = GET_ITEM_KEYCOUNT( player->items );
         if ( count < ITEM_MAXKEYS )
         {
            count++;
            SET_ITEM_KEYCOUNT( player->items, count );
            collected = True;
         }
         break;
      case ITEM_HERB:
         count = GET_ITEM_HERBCOUNT( player->items );
         if ( count < ITEM_MAXHERBS )
         {
            count++;
            SET_ITEM_HERBCOUNT( player->items, count );
            collected = True;
         }
         break;
      case ITEM_WING:
         count = GET_ITEM_WINGCOUNT( player->items );
         if ( count < ITEM_MAXWINGS )
         {
            count++;
            SET_ITEM_WINGCOUNT( player->items, count );
            collected = True;
         }
         break;
      case ITEM_FAIRYWATER:
         count = GET_ITEM_FAIRYWATERCOUNT( player->items );
         if ( count < ITEM_MAXFAIRYWATERS )
         {
            count++;
            SET_ITEM_FAIRYWATERCOUNT( player->items, count );
            collected = True;
         }
         break;
      case ITEM_TABLET: SET_ITEM_HASTABLET( player->items, True ); collected = True; break;
      case ITEM_STONEOFSUNLIGHT: SET_ITEM_HASSTONEOFSUNLIGHT( player->items, True ); collected = True; break;
   }

   return collected;
}

uint8_t Player_GetMapItemCount( Player_t* player )
{
   uint8_t count = 0;

   if ( GET_ITEM_KEYCOUNT( player->items ) ) count++;
   if ( GET_ITEM_HERBCOUNT( player->items ) ) count++;
   if ( GET_ITEM_WINGCOUNT( player->items ) ) count++;
   if ( GET_ITEM_FAIRYWATERCOUNT( player->items ) ) count++;
   if ( GET_ITEM_HASRAINBOWDROP( player->items ) ) count++;
   if ( GET_ITEM_HASSILVERHARP( player->items ) ) count++;
   if ( GET_ITEM_HASFAIRYFLUTE( player->items ) ) count++;
   if ( GET_ITEM_HASGWAELYNSLOVE( player->items ) ) count++;

   return count;
}
