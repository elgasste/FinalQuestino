using System.IO;
using System.Windows.Media;
using System.Windows;
using System.Windows.Media.Imaging;

var _palette = new List<ushort>();
var _textTextureMap = new List<byte>();
var _tileTextureMapBytes = new List<byte>();
var _playerSpriteTextureMapBytes = new List<byte>();

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

int PaletteIndexFromColor( ushort color )
{
   if ( _palette is null )
   {
      throw new Exception( "Somehow the palette is null, no idea how it happened." );
   }
   else if ( _palette.Count > 15 )
   {
      throw new Exception( "Trying to add too many colors to the palette." );
   }

   int paletteIndex = _palette.IndexOf( color );

   if ( paletteIndex < 0 )
   {
      _palette.Add( color );
      paletteIndex = _palette.Count - 1;
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
   else if ( ( bitmap.PixelHeight * bitmap.PixelWidth ) > ( 16 * 16 * 16 ) )
   {
      throw new Exception( "Trying to add too many tile textures." );
   }

   for ( int row = 0; row < bitmap.PixelHeight; row++ )
   {
      for ( int col = 0; col < bitmap.PixelWidth; col++ )
      {
         var pixelColor = GetPixelColor16( bitmap, col, row );
         _tileTextureMapBytes.Add( (byte)PaletteIndexFromColor( pixelColor ) );
      }
   }
}

void LoadPlayerSpriteTextureMap( BitmapSource bitmap )
{
   if ( _playerSpriteTextureMapBytes is null )
   {
      throw new Exception( "Somehow the player sprite texture map is null, no idea how it happened." );
   }

   for ( int row = 0; row < bitmap.PixelHeight; row++ )
   {
      for ( int col = 0; col < bitmap.PixelWidth; col++ )
      {
         var pixelColor = GetPixelColor16( bitmap, col, row );
         _playerSpriteTextureMapBytes.Add( (byte)PaletteIndexFromColor( pixelColor ) );
      }
   }
}

string BuildPaletteOutputString()
{
   if ( _palette is null )
   {
      throw new Exception( "Somehow the palette is null, I have no idea what could have happened." );
   }

   string outputString = "void cScreen_LoadPalette( cScreen_t* screen, uint8_t index )\n";
   outputString += "{\n";
   outputString += "   if ( index == 0 )\n";
   outputString += "   {\n";

   for ( int i = 0; i < 16; i++ )
   {
      if ( i < _palette.Count )
      {
         outputString += string.Format( "      screen->palette[{0}] = 0x{1};\n", i, _palette[i].ToString( "X4" ) );
      }
      else
      {
         outputString += string.Format( "      screen->palette[{0}] = 0x0000;\n", i );
      }
   }

   outputString += "   }\n";
   outputString += "}\n\n";

   return outputString;
}

string BuildTileTexturesOutputString()
{
   if ( _palette is null )
   {
      throw new Exception( "Somehow the palette is null, I have no idea what could have happened." );
   }
   else if ( _tileTextureMapBytes is null )
   {
      throw new Exception( "Somehow the tile texture map is null, I have no idea what went wrong." );
   }

   string outputString = "void cTileMap_LoadTileTextures( cTileMap_t* map, uint8_t index )\n";
   outputString += "{\n";
   outputString += "   if ( index == 0 )\n";
   outputString += "   {\n";

   int tileTextureCount = _tileTextureMapBytes.Count / 16 / 16;

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
            outputString += string.Format( "      map->tileTextures[{0}][{1}] = 0x{2};\n", i, counter, packedPixels.ToString( "X2" ) );
         }
      }
   }

   outputString += "   }\n";
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

   for ( int i = 0; i < DataGenerator.MapData.MapTiles.Count; i++ )
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

      for ( int j = 0; j < DataGenerator.MapData.MapTiles[i].Count; j++ )
      {
         outputString += string.Format( "      map->tiles[{0}] = 0x{1};\n", j, DataGenerator.MapData.MapTiles[i][j].ToString( "X2" ) );
      }

      outputString += string.Format( "      map->stride = {0};\n", DataGenerator.MapData.MapStrides[i] );

      for ( int j = 0; j < 4; j++ )
      {
         outputString += string.Format( "      map->portals[{0}] = 0x{1};\n", j, DataGenerator.MapData.MapPortals[i][j].CompiledData.ToString( "X2" ) );
      }

      outputString += "   }\n";
   }

   outputString += "}\n";

   return outputString;
}

void GenerateOutputData()
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
}

try
{
   string outputString = "/* This file was generated from DataGenerator, please do not edit */\n\n";
   outputString += "#include \"game.h\"\n\n";

   Console.Write( "Generating data..." );
   GenerateOutputData();
   Console.Write( "Done!\n" );

   Console.Write( "Generating palette loader..." );
   outputString += BuildPaletteOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating tile texture loader..." );
   outputString += BuildTileTexturesOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating text bit field loader..." );
   outputString += BuildTextBitFieldsOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating player sprite texture loader..." );
   outputString += BuildPlayerSpriteTexturesOutputString();
   Console.Write( "Done!\n" );

   Console.Write( "Generating map tiles..." );
   outputString += BuildMapTilesOutputString();
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
