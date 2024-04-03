namespace DataGenerator
{
   internal class MapEnemyTierData
   {
      public List<int> Indexes { get; }
      public List<int> SpecialIndexes { get; }

      public MapEnemyTierData( List<int> indexes, List<int> specialIndexes )
      {
         Indexes = indexes;
         SpecialIndexes = specialIndexes;
      }
   }
}
