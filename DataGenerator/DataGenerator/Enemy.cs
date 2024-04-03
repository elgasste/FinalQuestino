namespace DataGenerator
{
   internal class Enemy
   {
      public int Index = 0;
      public string Name = string.Empty;
      public int HitPoints = 0;
      public int MagicPoints = 0;
      public int AttackPower = 0;
      public int DefensePower = 0;
      public int Agility = 0;
      public int Experience = 0;
      public int Gold = 0;

      public List<ushort> Palette = new();
      public List<int> TextureIndexes = new();
      public List<List<byte>> TileTextures = new();
   }
}
