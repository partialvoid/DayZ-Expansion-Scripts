/**
 * config.cpp
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Config
 * @brief		This file handle expansion core configs
 **/
class CfgPatches
{
    class DayZExpansion_Worlds_ChernarusPlus_Rays
    {
        units[]={};
        weapons[]={};
        requiredVersion=0.1;
        requiredAddons[]= 
        { 
            "DZ_Data",
            "DZ_Worlds_Chernarusplus_World"
        };
    };
};

class CfgWorlds
{
    class Weather;

    class Overcast;
	class Weather1;
	class Weather2;
	class Weather3;
	class Weather4;
	class Weather5;

    class LightingNew;

    class C_Night;
    class C_Astro1;
    class C_Astro2;
    class C_Astro3;
    class C_Nautical1;
    class C_Nautical2;
    class C_Nautical3;
    class C_Civil1;
    class C_Civil2;
    class C_Civil3;
    class C_Day0;
    class C_Day4;
    class C_Day8;
    class C_Day12;
    class C_Day16;
    class C_Day35;
    class C_Day60;
    class O_Night;
    class O_Astro1;
    class O_Nautical1;
    class O_Civil1;
    class O_Civil2;
    class O_Day0;
    class O_Day4;
    class O_Day8;
    class O_Day12;
    class O_Day35;
    class O_Day60;
	
	class Settlement_Chernogorsk;
	class Settlement_Novodmitrovsk;
	class Settlement_Novoselki;
	class Settlement_Dubovo;
	class Settlement_Vysotovo;
	class Settlement_Zelenogorsk;
	class Settlement_Berezino;
	class Settlement_Elektrozavodsk;
	class Settlement_NovayaPetrovka;
	class Settlement_Gorka;
	class Settlement_Solnechny;
	class Settlement_StarySobor;
	class Settlement_Vybor;
	class Settlement_Severograd;
	class Settlement_Bor;
	class Settlement_Svetloyarsk;
	class Settlement_Krasnostav;
	class Settlement_ChernayaPolyana;
	class Settlement_Polyana;
	class Settlement_Tulga;
	class Settlement_Msta;
	class Settlement_Staroye;
	class Settlement_Shakhovka;
	class Settlement_Dolina;
	class Settlement_Orlovets;
	class Settlement_NovySobor;
	class Settlement_Kabanino;
	class Settlement_Mogilevka;
	class Settlement_Nadezhdino;
	class Settlement_Guglovo;
	class Settlement_Kamyshovo;
	class Settlement_Pusta;
	class Settlement_Dubrovka;
	class Settlement_VyshnayaDubrovka;
	class Settlement_Khelm;
	class Settlement_Olsha;
	class Settlement_Gvozdno;
	class Settlement_Grishino;
	class Settlement_Rogovo;
	class Settlement_Pogorevka;
	class Settlement_Pustoshka;
	class Settlement_Kozlovka;
	class Settlement_Karmanovka;
	class Settlement_Balota;
	class Settlement_Komarovo;
	class Settlement_Kamenka;
	class Settlement_Myshkino;
	class Settlement_Pavlovo;
	class Settlement_Lopatino;
	class Settlement_Vyshnoye;
	class Settlement_Prigorodki;
	class Settlement_Drozhino;
	class Settlement_Sosnovka;
	class Settlement_Nizhneye;
	class Settlement_Pulkovo;
	class Settlement_Berezhki;
	class Settlement_Turovo;
	class Settlement_BelayaPolyana;
	class Settlement_Dobroye;
	class Settlement_Nagornoye;
	class Settlement_Svergino;
	class Settlement_Ratnoye;
	class Settlement_Kamensk;
	class Settlement_Krasnoye;
	class Settlement_StaryYar;
	class Settlement_Polesovo;
	class Settlement_Tisy;
	class Settlement_Topolniki;
	class Settlement_Zaprudnoye;
	class Settlement_Sinystok;
	class Settlement_Vavilovo;
	class Settlement_Kumyrna;
	class Settlement_Kalinovka;
	class Settlement_Bogatyrka;
	class Settlement_SvyatoyRomanSkiResort;
	class Settlement_SKVSChBiathlonArena;
	class Settlement_Zvir;
	class Settlement_Zabolotye;
	class Settlement_Galkino;
	class Camp_Arsenovo;
	class Camp_Stroytel;
	class Camp_Romashka;
	class Camp_Kometa;
	class Camp_Druzhba;
	class Camp_Nadezhda;
	class Camp_YouthPioneer;
	class Camp_ProudChernarus;
	class Camp_Shkolnik;
	class Camp_Pobeda;
	class Camp_Metalurg;
	class Hill_Zelenayagora;
	class Local_Dichina;
	class Local_Novylug;
	class Local_Staryeluga;
	class Hill_Vysota;
	class Hill_Kopyto;
	class Local_Grubyeskaly;
	class Local_Chernyles;
	class Hill_Altar;
	class Local_RadioZenit;
	class Hill_PikKozlova;
	class Local_Pustoykhrebet;
	class Hill_Bashnya;
	class Hill_Veresnik;
	class Hill_Kurgan;
	class Hill_Kustryk;
	class Hill_Vetryanayagora;
	class Hill_Kalmyk;
	class Hill_PopIvan;
	class Hill_Erbenka;
	class Local_Lesnoykhrebet;
	class Hill_Vysokiykamen;
	class Hill_Dobry;
	class Hill_Baranchik;
	class Hill_Malinovka;
	class Hill_Dubina;
	class Hill_Klen;
	class Hill_Chernayagora;
	class Ruin_Zolotar;
	class Hill_Ostry;
	class Hill_Olsha;
	class Marine_Tikhiyzaliv;
	class Marine_Mutnyizaliv;
	class Marine_Chernyzaliv;
	class Marine_Zelenyzaliv;
	class Marine_Skalistyproliv;
	class Marine_Nizhniyzaliv;
	class Marine_ZalivGuba;
	class Marine_Rify;
	class Marine_Ivovoyeozero;
	class Marine_Chernoyeozero;
	class Marine_PlotinaTopolka;
	class Marine_PlotinaPobeda;
	class Marine_PlotinaTishina;
	class Marine_Ozerko;
	class Marine_Prud;
	class Ruin_Chortovzamok;
	class Ruin_Zub;
	class Ruin_Rog;
	class Local_Grozovypereval;
	class Local_Sosnovypereval;
	class Local_PerevalOreshka;
	class Local_Turovskiypereval;
	class Local_Tridoliny;
	class Local_Grozovyedoly;
	class Ruin_Klyuch;
	class Hill_Lysayagora;
	class Marine_Glubokoyeozero;
	class Local_Skalka;
	class Local_Vidy;
	class Hill_Tumannyverkh;
	class Local_Adamovka;
	class Hill_Shishok;
	class Settlement_Skalisty;
	class Ruin_Storozh;
	class Local_MysGolova;
	class Local_Drakon;
	class Local_Otmel;
	class Local_MysKrutoy;
	class Hill_Tokarnya;
	class Hill_Ostrog;
	class Local_Maryanka;
	class Local_Polonina;
	class Local_Kalinka;
	class Hill_Kikimora;
	class Hill_BolshoyKotel;
	class Hill_Simurg;
	class Hill_Volchiypik;
	class Ruin_Krona;
	class Local_TriKresta;
	class Local_Rostoki;
	class Marine_OrlyeOzero;
	class Local_Makosh;
	class Local_Klenovyipereval;
	class Local_Zmeinykhrebet;
	class Hill_Sokol;
	class Local_Krutyeskaly;
	class Local_Bogat;
	class Local_Dubnik;
	class Hill_Viselitsa;
	class Local_Dazhbog;
	class Marine_Verbnik;
	class Local_Medvezhilugi;
	class Ruin_Voron;
	class Ruin_Gnomovzamok;
	class Marine_Glaza;
	class Local_KarerKrasnayaZarya;
	class Local_Matveyevo;
	class Local_Kotka;
	class Local_Chernyeskaly;
	class Hill_SvyatoyRoman;
	class Hill_Koman;
	class Hill_Mayak;
	class Local_MB_VMC;
	class Local_MB_Tisy;
	class Local_MB_Zeleno;
	class Local_MB_Kamensk;
	class Local_MB_Pavlovo;
	class Local_AF_Balota;
	class Local_AF_Krasno;
	class Local_AF_Vybor;

    class VolFog;
	class Names;

    class SoundMapValues;
    class Subdivision;
    class Fractal;
    class WhiteNoise; 

    class DefaultLighting;
	
	class ChernarusPlus;
	class ChernarusPlusRays: ChernarusPlus
	{
		class Weather: Weather
		{
			dayTemperature=18;
			nightTemperature=4;
            minHeight=100;
            shoreTop=10;
            peakWaveTop=1000;
            peakWaveBottom=-1000;
			class VolFog: VolFog
			{
				CameraFog=1;
				Item1[]={800,0.059999999,0.93000001,0.13,1};
				Item2[]={1300,0.02,0.89999998,0.050000001,1};
			};
			class Overcast: Overcast
			{
				class Weather1: Weather1
				{
					overcast=0;
					lightingOvercast=0;
					sky="DZ\data\data\sky_clear_sky.paa";
					skyR="DZ\data\data\sky_clear_lco.paa";
					horizon="DZ\data\data\sky_clear_horizont_sky.paa";
					alpha=0;
					bright=0;
					speed=0.1;
					size=0.5;
					height=1;
					through=0;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
				class Weather2: Weather2
				{
					overcast=0.30000001;
					lightingOvercast=0.30000001;
					sky="DZ\data\data\sky_almostclear_sky.paa";
					skyR="DZ\data\data\sky_almostclear_lco.paa";
					horizon="DZ\data\data\sky_almostclear_horizont_sky.paa";
					alpha=0.1;
					bright=0.2;
					speed=0.30000001;
					size=0.55000001;
					height=0.89999998;
					through=0;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
				class Weather3: Weather3
				{
					overcast=0.5;
					lightingOvercast=0.5;
					sky="DZ\data\data\sky_semicloudy_sky.paa";
					skyR="DZ\data\data\sky_semicloudy_lco.paa";
					horizon="DZ\data\data\sky_semicloudy_horizont_sky.paa";
					alpha=0.25;
					bright=0.2;
					speed=0.5;
					size=0.64999998;
					height=0.85000002;
					through=0;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
				class Weather4: Weather4
				{
					overcast=0.69999999;
					lightingOvercast=0.69999999;
					sky="DZ\data\data\sky_cloudy_sky.paa";
					skyR="DZ\data\data\sky_cloudy_lco.paa";
					horizon="DZ\data\data\sky_cloudy_horizont_sky.paa";
					alpha=0.34999999;
					bright=0.2;
					speed=0.75;
					size=0.85000002;
					height=0.80000001;
					through=0;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
				class Weather5: Weather5
				{
					overcast=1.01;
					lightingOvercast=1;
					sky="DZ\data\data\sky_mostlycloudy_sky.paa";
					skyR="DZ\data\data\sky_mostlycloudy_lco.paa";
					horizon="DZ\data\data\sky_mostlycloudy_horizont_sky.paa";
					alpha=0.34999999;
					bright=0.1;
					speed=0.80000001;
					size=1;
					height=0.75;
					through=0;
					diffuse=1;
					cloudDiffuse=1;
					waves=100;
				};
			};
		};
		envTexture="DZ\Data\data\env_co.paa";
		clouds[]=
		{
			"DZ\Data\data\mrak1.p3d",
			"DZ\Data\data\mrak2.p3d",
			"DZ\Data\data\mrak3.p3d",
			"DZ\Data\data\mrak4.p3d"
		};
		skyObject="DZ\Data\data\obloha.p3d";
		starsObject="DZ\Data\data\stars.p3d";
		pointObject="DZ\Data\data\point.p3d";
		horizontObject="DZ\Data\data\horizont.p3d";
		haloObject="DZ\Data\data\sunHalo.p3d";
		sunObject="DZ\Data\data\sun.p3d";
		rainbowObject="DZ\Data\data\rainbow.p3d";
		moonObject="DZ\Data\data\moon.p3d";
		starsIntensity=0.141;
		hazeDistCoef=0;
		hazeFogCoef=1;
		rainMaterial="{AD76A616D0EF1ABC}Graphics/Materials/postprocess/rain.emat";
		satelliteNormalBlendStart=100;
		satelliteNormalBlendEnd=500;
		flareMinDistance=2;
		flareMinDistanceLerp=1;
		farLightSpotMul=2;
		farLightPointMul=2;
		minTreesInForestSquare=10;
		minRocksInRockSquare=5;
		soundMapAttenCoef=0.0099999998;
		class SoundMapValues: SoundMapValues
		{
			treehard=0.029999999;
			treesoft=0.029999999;
			bushhard=0;
			bushsoft=0;
			forest=1;
			house=0.30000001;
			church=1;
		};
		class Subdivision: Subdivision
		{
			class Fractal: Fractal
			{
				rougness=5;
				maxRoad=0.02;
				maxTrack=0.5;
				maxSlopeFactor=0.050000001;
			};
			class WhiteNoise: WhiteNoise
			{
				rougness=2;
				maxRoad=0.0099999998;
				maxTrack=0.050000001;
				maxSlopeFactor=0.0024999999;
			};
			minY=-0;
			minSlope=0.02;
		};
		//longitude=30;
		//latitude=-56;
		class Lighting: DefaultLighting
		{
			groundReflection[]={0.059999999,0.059999999,0.029999999};
            moonObjectColorFull[]={0.89999998,0.89999998,1,0.69999999};
            moonHaloObjectColorFull[]={0.89999998,0.89999998,1,0.0099999998};
            moonsetObjectColor[]={0.89999998,0.75,0.40000001};
            moonsetHaloObjectColor[]={0.89999998,0.5,0.2};
            nightAngle=5;
            sunSunset=20;
            endSunset=10;
		};
	};
};