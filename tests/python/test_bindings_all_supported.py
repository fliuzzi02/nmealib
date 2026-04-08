import pytest

import nmealib


@pytest.mark.parametrize(
    ("raw", "expected_type"),
    [
        ("$GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M,011,M*82\r\n", nmealib.nmea0183.APB),
        ("$SDDBT,15.5,f,4.7,M,2.6,F*2C\r\n", nmealib.nmea0183.DBT),
        ("$INDPT,2.3,0.0,100.0*73\r\n", nmealib.nmea0183.DPT),
        ("$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55\r\n", nmealib.nmea0183.GGA),
        ("$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76\r\n", nmealib.nmea0183.GLL),
        ("$GPGSA,A,3,04,05,09,12,24,25,29,31,32,37,43,46,1.80,1.00,1.50*10\r\n", nmealib.nmea0183.GSA),
        ("$GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74\r\n", nmealib.nmea0183.GSV),
        ("$IIHDG,274.1,1.2,E,3.4,W*58\r\n", nmealib.nmea0183.HDG),
        ("$IIHDM,45.0,M*13\r\n", nmealib.nmea0183.HDM),
        ("$IIMTW,23.5,C*1E\r\n", nmealib.nmea0183.MTW),
        ("$IIMWV,45.0,R,10.5,N,A*26\r\n", nmealib.nmea0183.MWV),
        ("$GPRMA,A,4917.24,N,12309.57,W,0.0,0.0,0.5,54.7,20.3,E*67\r\n", nmealib.nmea0183.RMA),
        ("$GPRMB,A,0.66,L,003,004,4917.24,N,12309.57,W,1.3,52.5,0.5,V*04\r\n", nmealib.nmea0183.RMB),
        ("$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n", nmealib.nmea0183.RMC),
        ("$IIVHW,220.86,T,218.90,M,2.550,N,4.724,K*5A\r\n", nmealib.nmea0183.VHW),
        ("$IIVLW,123.4,N,23.4,N,234.5,N,34.5,N*4E\r\n", nmealib.nmea0183.VLW),
        ("$GPVTG,220.86,T,218.90,M,2.550,N,4.724,K*40\r\n", nmealib.nmea0183.VTG),
        ("$IIVWR,45.0,R,10.5,N,5.4,M,19.4,K*55\r\n", nmealib.nmea0183.VWR),
        ("$HCXDR,A,171,D,PITCH,A,-37,D,ROLL,G,367,,MAGX,G,2420,,MAGY,G,-8984,,MAGZ*41\r\n", nmealib.nmea0183.XDR),
        ("$GPXTE,A,A,0.7,L,N*6F\r\n", nmealib.nmea0183.XTE),
        ("$GPZDA,160012.71,11,03,2004,-1,00*7D\r\n", nmealib.nmea0183.ZDA),
    ],
)
def test_all_supported_nmea0183_factory_dispatch(raw, expected_type):
    msg = nmealib.nmea0183.Nmea0183Factory.create(raw)
    assert isinstance(msg, expected_type)


@pytest.mark.parametrize(
    ("raw", "expected_type"),
    [
        ("09F11260:342C71FF7FFF7FFD", nmealib.nmea2000.PGN127250),
        ("01F50300:01F4012C01000000", nmealib.nmea2000.PGN128259),
        ("01F80100:0000000000000000", nmealib.nmea2000.PGN129025),
        ("01F80200:01010040F4010000", nmealib.nmea2000.PGN129026),
        ("19FD0201:8C2C010479FAFFFF", nmealib.nmea2000.PGN130306),
    ],
)
def test_all_supported_nmea2000_factory_dispatch(raw, expected_type):
    msg = nmealib.nmea2000.Nmea2000Factory.create(raw)
    assert isinstance(msg, expected_type)