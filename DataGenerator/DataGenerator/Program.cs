using System.IO;
using System.Windows.Media;
using System.Windows;
using System.Windows.Media.Imaging;
using DataGenerator;
using Newtonsoft.Json;

var _mapPalette = new List<ushort>();
var _textTextureMap = new List<byte>();
var _tileTextureMapBytes = new List<byte>();
var _playerSpriteTextureMapBytes = new List<byte>();
var EnemyRepo = new EnemyRepo();

Console.WriteLine();
Console.WriteLine( "=====================================================" );
Console.WriteLine( "            FINAL QUESTINO DATA GENERATOR" );
Console.WriteLine( "=====================================================" );
Console.WriteLine();

void BitmapSanityCheck( BitmapSource bitmap )
{
   if ( bitmap.Format != PixelFormats.Indexed8 )
   {
      throw new Exception( "Expecting image pixel format to be Indexed8" );
   }
   else if ( bitmap.PixelWidth <= 0 || bitmap.PixelWidth % 8 != 0 )
   {
      throw new Exception( "Image width should be a multiple of 8" );
   }
   else if ( bitmap.PixelHeight <= 0 || bitmap.PixelWidth % 8 != 0 )
   {
      throw new Exception( "Image height should be a multiple of 8" );
   }
}

static ushort ColortoUInt16( Color color )
{
   var r16 = (ushort)( ( color.R / 255.0 ) * 31 );
   var g16 = (ushort)( ( color.G / 255.0 ) * 63 );
   var b16 = (ushort)( ( color.B / 255.0 ) * 31 );

   return (ushort)( ( r16 << 11 ) | ( g16 << 5 ) | b16 );
}

static Color GetPixelColor( BitmapSource bitmap, int x, int y )
{
   var bytesPerPixel = (int)Math.Truncate( ( bitmap.Format.BitsPerPixel + 7.0 ) / 8.0 );
   int stride = bitmap.PixelWidth * bytesPerPixel;
   byte[] pixel = new byte[1];
   var rect = new Int32Rect( x, y, 1, 1 );

   bitmap.CopyPixels( rect, pixel, stride, 0 );
   return bitmap.Palette.Colors[pixel[0]];
}

static ushort GetPixelColor16( BitmapSource bitmap, int x, int y ) => ColortoUInt16( GetPixelColor( bitmap, x, y ) );

int PaletteIndexFromColor( ushort color, List<ushort> palette )
{
   if ( palette is null )
   {
      throw new Exception( "Somehow a palette is null, no idea how it happened." );
   }
   else if ( palette.Count > 15 )
   {
      throw new Exception( "Trying to add too many colors to a palette." );
   }

   int paletteIndex = palette.IndexOf( color );

   if ( paletteIndex < 0 )
   {
      palette.Add( color );
      paletteIndex = palette.Count - 1;
   }

   return paletteIndex;
}

void LoadTextTextureMap( BitmapSource bitmap )
{
   if ( _textTextureMap is null )
   {
      throw new Exception( "Somehow the text texture map is null, no idea how it happened." );
   }

   for ( int row = 0; row < bitmap.PixelHeight; row++ )
   {
      for ( int col = 0; col < bitmap.PixelWidth; col += 8 )
      {
         _textTextureMap.Add( 0x00 );

         for ( int i = 0; i < 8; i++ )
         {
            var pixelColor = GetPixelColor( bitmap, col + i, row );

            if ( !Color.AreClose( pixelColor, Color.FromArgb( 255, 0, 0, 0 ) ) )
            {
               _textTextureMap[^1] |= (byte)( 1 << ( 8 - i - 1 ) );
            }
         }
      }
   }
}

void LoadWorldTextureMap( BitmapSource bitmap )
{
   if ( _tileTextureMapBytes is null )
   {
      throw new Exception( "Somehow the world texture map is null, no idea how it happened." );
   }
   else if ( ( bitmap.PixelHeight * bitmap.PixelWidth ) > ( 16 * 16 * 19 ) )
   {
      throw new Exception( "Trying to add too many tile textures." );
   }
   else if ( _mapPalette is null )
   {
      throw new Exception( "Somehow the map palette is null, no idea how it happened." );
   }

   for ( int row = 0; row < bitmap.PixelHeight; row++ )
   {
      for ( int col = 0; col < bitmap.PixelWidth; col++ )
      {
         var pixelColor = GetPixelColor16( bitmap, col, row );
         _tileTextureMapBytes.Add( (byte)PaletteIndexFromColor( pixelColor, _mapPalette ) );
      }
   }
}

void LoadPlayerSpriteTextureMap( BitmapSource bitmap )
{
   if ( _playerSpriteTextureMapBytes is null )
   {
      throw new Exception( "Somehow the player sprite texture map is null, no idea how it happened." );
   }
   else if ( _mapPalette is null )
   {
      throw new Exception( "Somehow the map palette is null, no idea how it happened." );
   }

   for ( int row = 0; row < bitmap.PixelHeight; row++ )
   {
      for ( int col = 0; col < bitmap.PixelWidth; col++ )
      {
         var pixelColor = GetPixelColor16( bitmap, col, row );
         _playerSpriteTextureMapBytes.Add( (byte)PaletteIndexFromColor( pixelColor, _mapPalette ) );
      }
   }
}

void LoadEnemyTextureMap( Enemy enemy, BitmapSource bitmap )
{
   if ( ( bitmap.PixelHeight * bitmap.PixelWidth ) != ( 80 * 96 ) )
   {
      throw new Exception( "Enemy texture map is the wrong size." );
   }

   var textureMapBytes = new List<byte>();

   for ( int row = 0; row < bitmap.PixelHeight; row++ )
   {
      for ( int col = 0; col < bitmap.PixelWidth; col++ )
      {
         var pixelColor = GetPixelColor16( bitmap, col, row );
         textureMapBytes.Add( (byte)PaletteIndexFromColor( pixelColor, enemy.Palette ) );
      }
   }

   for ( int tileRow = 0; tileRow < 12; tileRow++ )
   {
      for ( int tileCol = 0; tileCol < 10; tileCol++ )
      {
         bool blankTexture = true;
         var tileTextureBytes = new List<byte>();

         for ( int pixelRow = 0; pixelRow < 8; pixelRow++ )
         {
            for ( int pixelCol = 0; pixelCol < 8; pixelCol++ )
            {
               int pixelIndex = ( tileRow * 10 * 8 * 8 ) + ( pixelRow * 10 * 8 ) + ( tileCol * 8 ) + pixelCol;
               tileTextureBytes.Add( textureMapBytes[pixelIndex] );
            }
         }

         foreach ( var textureByte in tileTextureBytes )
         {
            if ( textureByte != 0 )
            {
               blankTexture = false;
               break;
            }
         }

         if ( blankTexture )
         {
            enemy.TextureIndexes.Add( -1 );
         }
         else
         {
            // TODO: what if this tile texture already exists for this enemy? should we check for that?
            enemy.TileTextures.Add( tileTextureBytes );
            enemy.TextureIndexes.Add( enemy.TileTextures.Count - 1 );
         }
      }
   }
}

void LoadEnemies()
{
   using StreamReader r = new( "Enemies/enemies.json" );
   var json = r.ReadToEnd();
   EnemyRepo = JsonConvert.DeserializeObject<EnemyRepo>( json );

   foreach ( var enemy in EnemyRepo.enemies )
   {
      var enemyFileStream = new FileStream( "Enemies/" + enemy.Name + ".png", FileMode.Open, FileAccess.Read, FileShare.Read );
      var enemyDecoder = new PngBitmapDecoder( enemyFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
      BitmapSource enemyBitmap = enemyDecoder.Frames[0];
      BitmapSanityCheck( enemyBitmap );
      LoadEnemyTextureMap( enemy, enemyBitmap );
   }
}

string BuildMapPaletteOutputString()
{
   if ( _mapPalette is null )
   {
      throw new Exception( "Somehow the map palette is null, I have no idea what could have happened." );
   }

   string outputString = "void cScreen_LoadMapPalette( cScreen_t* screen, uint8_t index )\n";
   outputString += "{\n";
   outputString += "   if ( index == 0 )\n";
   outputString += "   {\n";

   for ( int i = 0; i < 16; i++ )
   {
      if ( i < _mapPalette.Count )
      {
         outputString += string.Format( "      screen->mapPalette[{0}] = 0x{1};\n", i, _mapPalette[i].ToString( "X4" ) );
      }
      else
      {
         outputString += string.Format( "      screen->mapPalette[{0}] = 0x0000;\n", i );
      }
   }

   outputString += "   }\n";
   outputString += "}\n\n";

   return outputString;
}

string BuildMapTileTexturesOutputString()
{
   if ( _mapPalette is null )
   {
      throw new Exception( "Somehow the map palette is null, I have no idea what could have happened." );
   }
   else if ( _tileTextureMapBytes is null )
   {
      throw new Exception( "Somehow the tile texture map is null, I have no idea what went wrong." );
   }

   string outputString = "void cTileTexture_LoadTileTextures( cTileMap_t* map )\n";
   outputString += "{\n";

   int tileTextureCount = _tileTextureMapBytes.Count / 16 / 16;

   // last time I checked, the most common value of packedPixels was 0x22 (by far),
   // so we can save some program space by pre-loading that value. if any of the
   // tile textures change in the future, this should be re-visited
   outputString += "   uint8_t i, j;\n\n";
   outputString += string.Format( "   for ( i = 0; i < {0}; i++ ) {{ for ( j = 0; j < 128; j++ ) {{ map->tileTextures[i].pixels[j] = 0x22; }} }}\n\n", tileTextureCount );

   for ( int i = 0; i < tileTextureCount; i++ )
   {
      for ( int row = 0, counter = 0; row < 16; row++ )
      {
         for ( int col = i * 16; col < ( i * 16 ) + 16; col += 2, counter++ )
         {
            int idx = col + ( row * ( tileTextureCount * 16 ) );
            var unpackedPixel1 = (ushort)_tileTextureMapBytes[idx++];
            var unpackedPixel2 = (ushort)_tileTextureMapBytes[idx];
            var packedPixels = (ushort)( ( unpackedPixel1 << 4 ) | unpackedPixel2 );

            if ( packedPixels != 0x22 )
            {
               outputString += string.Format( "   map->tileTextures[{0}].pixels[{1}] = 0x{2};\n", i, counter, packedPixels.ToString( "X2" ) );
            }
         }
      }

      ushort textureFlags = 0x0;

      // encounter rate (2 bits)
      switch ( i )
      {
         case 0:  // grass
         case 4:  // swamp
         case 9:  // bridge
         case 11: // brick
         case 13: // barrier
            textureFlags |= 0x1;
            break;
         case 1:  // forest
         case 2:  // hills
            textureFlags |= 0x2;
            break;
         case 3:  // desert
            textureFlags |= 0x3;
            break;
         default:
            break;
      }

      // walking speed (2 bits)
      switch ( i )
      {
         case 1:  // forest
         case 3:  // desert
            textureFlags |= 0x4;
            break;
         case 2:  // hills
         case 4:  // swamp
            textureFlags |= 0x8;
            break;
         case 13: // barrier
            textureFlags |= 0xC;
            break;
         default:
            break;
      }

      if ( i == 4 ) // causes damage
      {
         textureFlags |= 0x10;
      }
      else if ( i == 16 ) // causes super damage
      {
         textureFlags|= 0x20;
      }

      outputString += string.Format( "   map->tileTextures[{0}].flags = 0x{1};\n", i, textureFlags.ToString( "X2" ) );
   }

   outputString += "}\n\n";

   return outputString;
}

string BuildTextBitFieldsOutputString()
{
   if ( _textTextureMap is null )
   {
      throw new Exception( "Somehow the text texture map is null, I have no idea what went wrong." );
   }

   string outputString = "void cScreen_LoadTextBitFields( cScreen_t* screen )\n";
   outputString += "{\n";

   int charCount = _textTextureMap.Count / 8;

   for ( int ch = 0; ch < charCount; ch++ )
   {
      for ( int row = 0; row < 8; row++ )
      {
         byte b = _textTextureMap[ch + ( row * charCount )];
         outputString += string.Format( "   screen->textBitFields[{0}][{1}] = 0x{2};\n", ch, row, b.ToString( "X2" ) );
      }
   }

   outputString += "}\n\n";

   return outputString;
}

string BuildPlayerSpriteTexturesOutputString()
{
   if ( _playerSpriteTextureMapBytes is null )
   {
      throw new Exception( "Somehow the player sprite texture map is null, I have no idea what went wrong." );
   }

   string outputString = "void cPlayer_LoadSprite( cPlayer_t* player )\n";
   outputString += "{\n";

   int frameCount = _playerSpriteTextureMapBytes.Count / ( 4 * 16 ) / 16;

   for ( int row = 0, spriteIdx = 0; row < 4; row++ )
   {
      for ( int frame = 0; frame < frameCount; frame++ )
      {
         for ( int pixelRow = 0; pixelRow < 16; pixelRow++ )
         {
            for ( int pixelCol = 0; pixelCol < 16; pixelCol++, spriteIdx++ )
            {
               int rowStart = row * frameCount * 16 * 16;
               int frameOffset = frame * 16;
               int pixelIndex1 = rowStart + frameOffset + pixelCol++ + ( pixelRow * frameCount * 16 );
               int pixelIndex2 = rowStart + frameOffset + pixelCol + ( pixelRow * frameCount * 16 );
               var unpackedPixel1 = (ushort)_playerSpriteTextureMapBytes[pixelIndex1];
               var unpackedPixel2 = (ushort)_playerSpriteTextureMapBytes[pixelIndex2];
               var packedPixels = (ushort)( ( unpackedPixel1 << 4 ) | unpackedPixel2 );

               outputString += string.Format( "   player->sprite.frameTextures[{0}] = 0x{1};\n", spriteIdx, packedPixels.ToString( "X2" ) );
            }
         }
      }
   }

   outputString += "}\n\n";

   return outputString;
}

string BuildMapTilesOutputString()
{
   string outputString = "void cTileMap_LoadTileMap( cTileMap_t* map, uint8_t index )\n";
   outputString += "{\n";
   outputString += "   uint16_t i;\n\n";

   for ( int i = 0; i < MapData.MapTiles.Count; i++ )
   {
      if ( i == 0 )
      {
         outputString += "   if ( index == " + i + " )\n";
      }
      else
      {
         outputString += "   else if ( index == " + i + " )\n";
      }
      outputString += "   {\n";

      var tileCounts = new Dictionary<int, int>();

      for ( int j = 0; j < MapData.MapTiles.Count; j++ )
      {
         if ( tileCounts.ContainsKey( MapData.MapTiles[i][j] ) )
         {
            tileCounts[MapData.MapTiles[i][j]]++;
         }
         else
         {
            tileCounts[MapData.MapTiles[i][j]] = 1;
         }
      }

      int highestCount = 0;
      int mostCommonTile = -1;

      foreach ( var tile in tileCounts.Keys )
      {
         if ( tileCounts[tile] > highestCount )
         {
            highestCount = tileCounts[tile];
            mostCommonTile = tile;
         }
      }

      outputString += string.Format( "      for ( i = 0; i < {0}; i++ ) {{ map->tiles[i] = 0x{1}; }}\n", MapData.MapTiles[i].Count, mostCommonTile.ToString( "X2" ) );

      for ( int j = 0; j < MapData.MapTiles[i].Count; j++ )
      {
         if ( MapData.MapTiles[i][j] != mostCommonTile )
         {
            outputString += string.Format( "      map->tiles[{0}] = 0x{1};\n", j, MapData.MapTiles[i][j].ToString( "X2" ) );
         }
      }

      outputString += string.Format( "      map->stride = {0};\n", MapData.MapStrides[i] );

      for ( int j = 0; j < 4; j++ )
      {
         outputString += string.Format( "      map->portals[{0}] = 0x{1};\n", j, MapData.MapPortals[i][j].CompiledData.ToString( "X2" ) );
      }

      for ( int j = 0; j < 4; j++ )
      {
         outputString += string.Format( "      map->enemyIndexes[{0}] = {1};\n", j, MapData.MapEnemyIndexes[i].Indexes[j] );
      }
      for ( int j = 0; j < 4; j++ )
      {
         outputString += string.Format( "      map->enemySpecialIndexes[{0}] = {1};\n", j, MapData.MapEnemyIndexes[i].SpecialIndexes[j] );
      }

      outputString += string.Format( "      map->enemySpecialRegion.x = {0};\n", MapData.MapEnemySpecialRegions[i].Left );
      outputString += string.Format( "      map->enemySpecialRegion.y = {0};\n", MapData.MapEnemySpecialRegions[i].Top );
      outputString += string.Format( "      map->enemySpecialRegion.w = {0};\n", MapData.MapEnemySpecialRegions[i].Width );
      outputString += string.Format( "      map->enemySpecialRegion.h = {0};\n", MapData.MapEnemySpecialRegions[i].Height );

      outputString += "   }\n";
   }

   outputString += "}\n";

   return outputString;
}

string BuildEnemyOutputString()
{
   var sortedEnemies = EnemyRepo.enemies.OrderBy( e => e.Index ).ToList();

   string outputString = "\nvoid cEnemy_Load( cEnemy_t* enemy, uint8_t index )\n";
   outputString += "{\n";
   outputString += "   uint8_t i, j;\n\n";
   bool first = true;

   outputString += "   for ( i = 0; i < 78; i++ ) { for ( j = 0; j < 32; j++ ) { enemy->tileTextures[i][j] = 0; } };\n";

   foreach ( var enemy in sortedEnemies )
   {
      if ( first )
      {
         outputString += string.Format( "   if ( index == {0} )\n", enemy.Index );
         first = false;
      }
      else
      {
         outputString += string.Format( "   else if ( index == {0} )\n", enemy.Index );
      }

      outputString += "   {\n";
      outputString += string.Format( "      snprintf( enemy->name, 16, \"{0}\" );\n", enemy.Name );
      outputString += string.Format( "      enemy->indefiniteArticle = cIndefiniteArticle_{0};\n", enemy.IndefiniteArticle );
      outputString += string.Format( "      enemy->stats.HitPoints = {0};\n", enemy.HitPoints );
      outputString += string.Format( "      enemy->stats.MaxHitPoints = {0};\n", enemy.HitPoints );
      outputString += string.Format( "      enemy->stats.MagicPoints = {0};\n", 255 );
      outputString += string.Format( "      enemy->stats.MaxMagicPoints = {0};\n", enemy.MagicPoints );
      outputString += string.Format( "      enemy->stats.AttackPower = {0};\n", enemy.AttackPower );
      outputString += string.Format( "      enemy->stats.DefensePower = {0};\n", enemy.DefensePower );
      outputString += string.Format( "      enemy->stats.Agility = {0};\n", enemy.Agility );

      for ( int i = 0; i < enemy.Palette.Count; i++ )
      {
         outputString += string.Format( "      enemy->palette[{0}] = 0x{1};\n", i, enemy.Palette[i].ToString( "X4" ) );
      }

      outputString += string.Format( "      for ( i = 0; i < {0}; i++ ) {{ enemy->tileTextureIndexes[i] = -1; }}\n", enemy.TextureIndexes.Count );

      for ( int i = 0; i < enemy.TextureIndexes.Count; i++ )
      {
         if ( enemy.TextureIndexes[i] >= 0 )
         {
            outputString += string.Format( "      enemy->tileTextureIndexes[{0}] = {1};\n", i, enemy.TextureIndexes[i] );
         }
      }

      for ( int i = 0; i < enemy.TileTextures.Count; i++ )
      {
         for ( int j = 0, idx = 0; j < enemy.TileTextures[i].Count; j++, idx++ )
         {
            var unpackedPixel1 = (ushort)enemy.TileTextures[i][j++];
            var unpackedPixel2 = (ushort)enemy.TileTextures[i][j];
            var packedPixels = (ushort)( ( unpackedPixel1 << 4 ) | unpackedPixel2 );

            if ( packedPixels != 0 )
            {
               outputString += string.Format( "      enemy->tileTextures[{0}][{1}] = 0x{2};\n", i, idx, packedPixels.ToString( "X2" ) );
            }
         }
      }

      outputString += "   }\n";
   }

   outputString += "}\n";

   return outputString;
}

void LoadData()
{
   var textFileStream = new FileStream( "text_tileset.png", FileMode.Open, FileAccess.Read, FileShare.Read );
   var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
   BitmapSource textBitmap = textDecoder.Frames[0];
   BitmapSanityCheck( textBitmap );
   LoadTextTextureMap( textBitmap );

   var worldFileStream = new FileStream( "map_tileset.png", FileMode.Open, FileAccess.Read, FileShare.Read );
   var worldDecoder = new PngBitmapDecoder( worldFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
   BitmapSource worldBitmap = worldDecoder.Frames[0];
   BitmapSanityCheck( worldBitmap );
   LoadWorldTextureMap( worldBitmap );

   var playerSpriteFileStream = new FileStream( "player_sprite_tileset.png", FileMode.Open, FileAccess.Read, FileShare.Read );
   var playerSpriteDecoder = new PngBitmapDecoder( playerSpriteFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
   BitmapSource playerSpriteBitmap = playerSpriteDecoder.Frames[0];
   BitmapSanityCheck( playerSpriteBitmap );
   LoadPlayerSpriteTextureMap( playerSpriteBitmap );

   LoadEnemies();
}

try
{
   string outputString = "/* This file was generated from DataGenerator, please do not edit */\n\n";
   outputString += "#include \"game.h\"\n\n";

   Console.Write( "Loading data..." );
   LoadData();
   Console.Write( "Done!\n" );

   Console.Write( "Generating map palette loader..." );
   outputString += BuildMapPaletteOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating map tile texture loader..." );
   outputString += BuildMapTileTexturesOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating text bit field loader..." );
   outputString += BuildTextBitFieldsOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating player sprite texture loader..." );
   outputString += BuildPlayerSpriteTexturesOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating map data loader..." );
   outputString += BuildMapTilesOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating enemy data loader..." );
   outputString += BuildEnemyOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Writing source file..." );
   File.WriteAllText( "data_loader.c", outputString );
   Console.Write( "Done!\n" );

   Console.Write( "Copying source file..." );
   File.Copy( "data_loader.c", "..\\..\\..\\..\\..\\FinalQuestino\\data_loader.c", overwrite: true );
   Console.Write( "Done!\n\n" );

   Console.WriteLine( "Everything worked out perfectly with no possibility of errors. Bye!" );
}
catch( Exception e )
{
   Console.WriteLine( "\n\nUh-oh, something went wrong: " + e.Message + "\n" );
}
