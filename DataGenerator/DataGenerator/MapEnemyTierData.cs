namespace DataGenerator
{
   internal class MapEnemyTierData
   {
      public int IndexCount { get; }
      public List<int> Indexes { get; }

      public MapEnemyTierData( int indexCount, List<int> indexes )
      {
         IndexCount = indexCount;
         Indexes = indexes;
      }
   }
}
