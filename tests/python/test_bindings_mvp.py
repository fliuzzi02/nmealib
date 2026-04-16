import math

import nmealib


RMC_SENTENCE = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n"
GGA_SENTENCE = "$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n"
GLL_SENTENCE = "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n"
VTG_SENTENCE = "$GPVTG,220.86,T,218.90,M,2.550,N,4.724,K*40\r\n"
PGN127250_SENTENCE = "09F11260:342C71FF7FFF7FFD"
PGN127245_SENTENCE = "01F10D00:0101FEBF0060FFFF"
PGN127257_SENTENCE = "01F11900:02FE3F0060FE9F00"
PGN128001_SENTENCE = "01F40100:050A00FBFF6400FF"
PGN129026_SENTENCE = "01F80200:01010040F4010000"


def test_create_from_fields_0183():
    rmc = nmealib.nmea0183.RMC("GP", 123519, "A", 48.1173, "N", 11.5166, "E", 22.4, 84.4, 230394, 3.1, "W", "A", "V")
    assert rmc.get_sentence_type() == "RMC"
    assert math.isclose(rmc.get_latitude(), 48.1173, rel_tol=0, abs_tol=1e-6)
    assert math.isclose(rmc.get_longitude(), 11.5166, rel_tol=0, abs_tol=1e-6)

    gga = nmealib.nmea0183.GGA("GN", 62735.0, 31.8464692667, "N", 117.1987063833, "E", 1, 12, 2.0, 90.0, "M", 0.0, "M", 0.0, "")
    assert gga.get_sentence_type() == "GGA"

    gll = nmealib.nmea0183.GLL("GN", 31.8464692667, "N", 117.1987063833, "E", 62735.0, "A", "A")
    assert gll.get_sentence_type() == "GLL"

    vtg = nmealib.nmea0183.VTG("GP", 220.86, 218.90, 2.55, 4.724)
    assert vtg.get_sentence_type() == "VTG"


def test_create_from_raw_factories_and_dispatch_0183():
    rmc = nmealib.nmea0183.Nmea0183Factory.create(RMC_SENTENCE)
    assert isinstance(rmc, nmealib.nmea0183.RMC)

    gga = nmealib.nmea0183.Nmea0183Factory.create(GGA_SENTENCE)
    assert isinstance(gga, nmealib.nmea0183.GGA)

    gll = nmealib.nmea0183.Nmea0183Factory.create(GLL_SENTENCE)
    assert isinstance(gll, nmealib.nmea0183.GLL)

    vtg = nmealib.nmea0183.Nmea0183Factory.create(VTG_SENTENCE)
    assert isinstance(vtg, nmealib.nmea0183.VTG)


def test_create_from_fields_2000():
    pgn127245 = nmealib.nmea2000.PGN127245(1, 1, 1.5707, -0.7853)
    assert pgn127245.get_pgn() == 127245

    pgn127250 = nmealib.nmea2000.PGN127250(2, 1.5707, -0.7853, 0.7853, 1)
    assert pgn127250.get_pgn() == 127250

    pgn127257 = nmealib.nmea2000.PGN127257(2, 1.5707, -0.7853, 0.7853)
    assert pgn127257.get_pgn() == 127257

    pgn128001 = nmealib.nmea2000.PGN128001(5, 0.1, -0.05, 1.0)
    assert pgn128001.get_pgn() == 128001

    pgn129026 = nmealib.nmea2000.PGN129026(7, 1, 1.5707, 5.0)
    assert pgn129026.get_pgn() == 129026


def test_create_from_raw_factories_and_dispatch_2000():
    pgn127245 = nmealib.nmea2000.Nmea2000Factory.create(PGN127245_SENTENCE)
    assert isinstance(pgn127245, nmealib.nmea2000.PGN127245)

    pgn127250 = nmealib.nmea2000.Nmea2000Factory.create(PGN127250_SENTENCE)
    assert isinstance(pgn127250, nmealib.nmea2000.PGN127250)

    pgn127257 = nmealib.nmea2000.Nmea2000Factory.create(PGN127257_SENTENCE)
    assert isinstance(pgn127257, nmealib.nmea2000.PGN127257)

    pgn128001 = nmealib.nmea2000.Nmea2000Factory.create(PGN128001_SENTENCE)
    assert isinstance(pgn128001, nmealib.nmea2000.PGN128001)

    pgn129026 = nmealib.nmea2000.Nmea2000Factory.create(PGN129026_SENTENCE)
    assert isinstance(pgn129026, nmealib.nmea2000.PGN129026)
