﻿namespace DataGenerator
{
   internal class Enemy
   {
      public int index = 0;
      public string name = string.Empty;
      public int hp = 0;
      public int mp = 0;
      public int atk = 0;
      public int def = 0;
      public int agl = 0;
      public int exp = 0;
      public int gold = 0;

      public List<ushort> palette = new();
      public List<int> textureIndexes = new();
      public List<List<byte>> tileTextures = new();
   }
}