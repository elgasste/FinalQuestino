namespace DataGenerator
{
   internal class PortalData
   {
      public UInt32 CompiledData { get; }

      public PortalData()
      {
         CompiledData = 0x7FFFFFFF;
      }

      public PortalData( UInt32 originTileIndex, UInt32 destinationTileMapIndex, UInt32 destinationTileIndex )
      {
         CompiledData = destinationTileIndex | ( destinationTileMapIndex << 11 ) | ( originTileIndex << 21 );
      }
   }
}
