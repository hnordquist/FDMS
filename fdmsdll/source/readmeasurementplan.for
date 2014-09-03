	subroutine updatemeasurementsbg(

c	input
     + mfilename, ! character*280
     + dbheader,  ! character*200 mp.csv file header
     + mday,
     + mmonth,
     + myear,
     + na,        ! real*4 neutrons A
     + nb,        ! real*4 neutrons B
     + nc,        ! real*4 neutrons C
     + gamma1,    ! real*4 gamma dose 1
     + gamma2,    ! real*4 gamma dose 2
     + detector   ! character*32 array(1000) detector IDs

     + )
 
      !DEC$ ATTRIBUTES DLLEXPORT :: updatemeasurementsbg
     
	implicit none

c	begin input variables

	character mfilename*280
	character dbheader*200
	integer mday,mmonth,myear
	real*4 na,nb,nc
	real*4 gamma1,gamma2
	character detector*32

c	end input variables

c	begin internal variable


	character string*200,f*32

c	end internal variable

	open(1,file=mfilename,status='unknown')
	read(1,'(a200)',err=1) string
	goto 10
1	write(1,'(a200)') dbheader
10	read(1,'(a200)',err=99,end=99) string
	goto 10
	
99	 string(1:21)='                    ,'
	 string(22:41)='Background          '
	 string(42:69)=', , ,    ,      ,  ,  ,    ,'
	 string(70:83)=' ,     ,     ,'
	 if (mday.lt.1) then
	  string(84:85)='  '
	 elseif (mday.lt.10) then
	  string(84:84)=' '
	  write(string(85:85),'(i1)') mday
	 else
	  write(string(84:85),'(i2)') mday
	 endif
	 string(86:86)=','
	 if (mmonth.lt.1) then
	  string(87:88)='  '
	 elseif (mmonth.lt.10) then
	  string(87:87)=' '
	  write(string(88:88),'(i1)') mmonth
	 else
	  write(string(87:88),'(i2)') mmonth
	 endif
	 string(89:89)=','
	 if (myear.lt.1900) then
	  string(90:93)='    '
	 else
        write(string(90:93),'(i4)') myear
	 endif
	 string(94:101)=',      ,'
	 write(string(102:110),'(f9.2)') na
	 string(111:111)=','
	 write(string(112:120),'(f9.2)') nb
	 string(121:121)=','
	 write(string(122:130),'(f9.2)') nc
	 string(131:131)=','
	 write(string(132:141),'(f10.4)') gamma1
	 string(142:142)=','
	 write(string(143:152),'(f10.4)') gamma2
	 string(153:153)=','
	 string(154:185)=detector
	 write(1,'(a185)') string
	
	close(1)

	end

c	*********************************************************************

	subroutine updatemeasurements(

c	input
     + mfilename, ! character*280
     + i,         ! integer member to be added
     + dbheader,  ! character*200 mp.csv file header
     + facility,  ! character*32 array(1000) facility names
     + id,        ! character*32 array(1000) item IDs
     + mtype,     ! integer array(1000) measurement type
                  ! 1 cycle verification
	            ! 2 assembly verification
	            ! 3 item reverification
     + mstatus,   ! integer array(1000)
     + en,        ! real*4 array(1000) 235U enrichment level in %
     + bu,        ! real*4 array(1000) declared burnup in GWd/MT
     + dcday,     ! integer array(1000) dischange day 1-31
     + dcmonth,   ! integer array(1000) dischange day 1-12
     + dcyear,    ! integer array(1000) dischange day 1900-3000
     + icycle,    ! integer array(1000)
     + thresA,    ! real*4 array(1000) threshold for neutron channel A
     + thresB,    ! real*4 array(1000) threshold for neutron channel B
     + mday,      ! integer array(1000) measurement day 1-31
     + mmonth,    ! integer array(1000) measurement month 1-12
     + myear,     ! integer array(1000) measurement year 1900-3000
     + ct,        ! real*4 array(1000) declared cooling time in years
     + na,        ! real*4 array(1000) neutrons A
     + nb,        ! real*4 array(1000) neutrons B
     + nc,        ! real*4 array(1000) neutrons C
     + gamma1,    ! real*4 array(1000) gamma dose 1
     + gamma2,    ! real*4 array(1000) gamma dose 2
     + detector   ! character*32 array(1000) detector IDs

     + )
 
      !DEC$ ATTRIBUTES DLLEXPORT :: updatemeasurements
     
	implicit none

c	begin input variables

	character mfilename*280
	integer i
	character dbheader*200
	character facility*32(1000),id*32(1000)
	integer mtype(1000),mstatus(1000),icycle(1000)
	integer dcday(1000),dcmonth(1000),dcyear(1000)
	integer mday(1000),mmonth(1000),myear(1000)
	real*4 en(1000),bu(1000),thresA(1000),thresB(1000)
	real*4 ct(1000),na(1000),nb(1000),nc(1000)
	real*4 gamma1(1000),gamma2(1000)
	character detector*32(1000)

c	end input variables

c	begin internal variable


	character string*200,f*32

c	end internal variable

	open(1,file=mfilename,status='unknown')
	read(1,'(a200)',err=1) string
	goto 10
1	write(1,'(a200)') dbheader
10	read(1,'(a200)',err=99,end=99) string
	goto 10
	
99	 string(1:20)=facility(i)
	 string(21:21)=','
	 string(22:41)=id(i)
	 string(42:42)=','
	 write(string(43:43),'(i1)') mtype(i)
	 string(44:44)=','
	 write(string(45:45),'(i1)') mstatus(i)
	 string(46:46)=','
	 write(string(47:50),'(f4.2)') en(i)
	 string(51:51)=','
	 write(string(52:57),'(f6.2)') bu(i)
	 string(58:58)=','
	 if (dcday(i).lt.1) then
	  string(59:60)='  '
	 elseif (dcday(i).lt.10) then
	  string(59:59)=' '
	  write(string(60:60),'(i1)') dcday(i)
	 else
	  write(string(59:60),'(i2)') dcday(i)
	 endif
	 string(61:61)=','
	 if (dcmonth(i).lt.1) then
	  string(62:63)='  '
	 elseif (dcmonth(i).lt.10) then
	  string(62:62)=' '
	  write(string(63:63),'(i1)') dcmonth(i)
	 else
	  write(string(62:63),'(i2)') dcmonth(i)
	 endif
	 string(64:64)=','
	 if (dcyear(i).lt.1900) then
	  string(65:68)='    '
	 else
        write(string(65:68),'(i4)') dcyear(i)
	 endif
	 string(69:69)=','
	 write(string(70:70),'(i1)') icycle(i)
	 string(71:71)=','
	 write(string(72:76),'(f5.1)') thresA(i)
	 string(77:77)=','
	 write(string(78:84),'(f7.1)') thresB(i)
	 string(85:85)=','
	 if (mday(i).lt.1) then
	  string(86:87)='  '
	 elseif (mday(i).lt.10) then
	  string(86:86)=' '
	  write(string(87:87),'(i1)') mday(i)
	 else
	  write(string(86:87),'(i2)') mday(i)
	 endif
	 string(88:88)=','
	 if (mmonth(i).lt.1) then
	  string(89:90)='  '
	 elseif (mmonth(i).lt.10) then
	  string(89:89)=' '
	  write(string(90:90),'(i1)') mmonth(i)
	 else
	  write(string(89:90),'(i2)') mmonth(i)
	 endif
	 string(91:91)=','
	 if (myear(i).lt.1900) then
	  string(92:95)='    '
	 else
        write(string(92:95),'(i4)') myear(i)
	 endif
	 string(96:96)=','
	 write(string(97:102),'(f6.2)') ct(i)
	 string(103:103)=','
	 write(string(104:112),'(f9.2)') na(i)
	 string(113:113)=','
	 write(string(114:122),'(f9.2)') nb(i)
	 string(123:123)=','
	 write(string(124:132),'(f9.2)') nc(i)
	 string(133:133)=','
	 write(string(134:143),'(f10.4)') gamma1(i)
	 string(144:144)=','
	 write(string(145:154),'(f10.4)') gamma2(i)
	 string(155:155)=','
	 string(156:187)=detector(i)
	 write(1,'(a187)') string
	close(1)

	end
	
c	********************************************************************

	subroutine writemeasurementplan(

c	input
     + mpfilename,! character*280
     + ii,        ! integer number of entries in database.csv
     + dbheader,  ! character*200 mp.csv file header
     + facility,  ! character*32 array(1000) facility names
     + id,        ! character*32 array(1000) item IDs
     + mtype,     ! integer array(1000) measurement type
                  ! 1 cycle verification
	            ! 2 assembly verification
	            ! 3 item reverification
     + mstatus,   ! integer array(1000)
     + en,        ! real*4 array(1000) 235U enrichment level in %
     + bu,        ! real*4 array(1000) declared burnup in GWd/MT
     + dcday,     ! integer array(1000) dischange day 1-31
     + dcmonth,   ! integer array(1000) dischange day 1-12
     + dcyear,    ! integer array(1000) dischange day 1900-3000
     + icycle,    ! integer array(1000)
     + thresA,    ! real*4 array(1000) threshold for neutron channel A
     + thresB,    ! real*4 array(1000) threshold for neutron channel B
     + mday,      ! integer array(1000) measurement day 1-31
     + mmonth,    ! integer array(1000) measurement month 1-12
     + myear,     ! integer array(1000) measurement year 1900-3000
     + ct,        ! real*4 array(1000) declared cooling time in years
     + na,        ! real*4 array(1000) neutrons A
     + nb,        ! real*4 array(1000) neutrons B
     + nc,        ! real*4 array(1000) neutrons C
     + gamma1,    ! real*4 array(1000) gamma dose 1
     + gamma2,    ! real*4 array(1000) gamma dose 2
     + detector   ! character*32 array(1000) detector IDs

     + )
 
      !DEC$ ATTRIBUTES DLLEXPORT :: writemeasurementplan
     
	implicit none

c	begin input variables

	character mpfilename*280
	integer ii
	character dbheader*200
	character facility*32(1000),id*32(1000)
	integer mtype(1000),mstatus(1000),icycle(1000)
	integer dcday(1000),dcmonth(1000),dcyear(1000)
	integer mday(1000),mmonth(1000),myear(1000)
	real*4 en(1000),bu(1000),thresA(1000),thresB(1000)
	real*4 ct(1000),na(1000),nb(1000),nc(1000)
	real*4 gamma1(1000),gamma2(1000)
	character detector*32(1000)

c	end input variables

c	begin internal variable

	integer i
	character string*200,f*32

c	end internal variable

	open(1,file=mpfilename,status='unknown')
	write(1,'(a200)') dbheader
	do i=1,ii
	 string(1:20)=facility(i)
	 string(21:21)=','
	 string(22:41)=id(i)
	 string(42:42)=','
	 write(string(43:43),'(i1)') mtype(i)
	 string(44:44)=','
	 write(string(45:45),'(i1)') mstatus(i)
	 string(46:46)=','
	 write(string(47:50),'(f4.2)') en(i)
	 string(51:51)=','
	 write(string(52:57),'(f6.2)') bu(i)
	 string(58:58)=','
	 if (dcday(i).lt.1) then
	  string(59:60)='  '
	 elseif (dcday(i).lt.10) then
	  string(59:59)=' '
	  write(string(60:60),'(i1)') dcday(i)
	 else
	  write(string(59:60),'(i2)') dcday(i)
	 endif
	 string(61:61)=','
	 if (dcmonth(i).lt.1) then
	  string(62:63)='  '
	 elseif (dcmonth(i).lt.10) then
	  string(62:62)=' '
	  write(string(63:63),'(i1)') dcmonth(i)
	 else
	  write(string(62:63),'(i2)') dcmonth(i)
	 endif
	 string(64:64)=','
	 if (dcyear(i).lt.1900) then
	  string(65:68)='    '
	 else
        write(string(65:68),'(i4)') dcyear(i)
	 endif
	 string(69:69)=','
	 write(string(70:70),'(i1)') icycle(i)
	 string(71:71)=','
	 write(string(72:76),'(f5.1)') thresA(i)
	 string(77:77)=','
	 write(string(78:84),'(f7.1)') thresB(i)
	 string(85:85)=','
	 if (mday(i).lt.1) then
	  string(86:87)='  '
	 elseif (mday(i).lt.10) then
	  string(86:86)=' '
	  write(string(87:87),'(i1)') mday(i)
	 else
	  write(string(86:87),'(i2)') mday(i)
	 endif
	 string(88:88)=','
	 if (mmonth(i).lt.1) then
	  string(89:90)='  '
	 elseif (mmonth(i).lt.10) then
	  string(89:89)=' '
	  write(string(90:90),'(i1)') mmonth(i)
	 else
	  write(string(89:90),'(i2)') mmonth(i)
	 endif
	 string(91:91)=','
	 if (myear(i).lt.1900) then
	  string(92:95)='    '
	 else
        write(string(92:95),'(i4)') myear(i)
	 endif
	 string(96:96)=','
	 write(string(97:102),'(f6.2)') ct(i)
	 string(103:103)=','
	 write(string(104:112),'(f9.2)') na(i)
	 string(113:113)=','
	 write(string(114:122),'(f9.2)') nb(i)
	 string(123:123)=','
	 write(string(124:132),'(f9.2)') nc(i)
	 string(133:133)=','
	 write(string(134:143),'(f10.4)') gamma1(i)
	 string(144:144)=','
	 write(string(145:154),'(f10.4)') gamma2(i)
	 string(155:155)=','
	 string(156:187)=detector(i)
	 write(1,'(a187)') string
	enddo
	
	close(1)

	end

c	******************************************************************************

	subroutine readmeasurementplan(

c	outputs
     + mpfilename,! character*280
     + i,         ! integer number of entries in database.csv
     + dbheader,  ! character*200 database.csv file header
     + facility,  ! character*32 array(1000) facility names
     + id,        ! character*32 array(1000) item IDs
     + mtype,     ! integer array(1000) measurement type
                  ! 1 cycle verification
	            ! 2 assembly verification
	            ! 3 item reverification
     + mstatus,   ! integer array(1000)
     + en,        ! real*4 array(1000) 235U enrichment level in %
     + bu,        ! real*4 array(1000) declared burnup in GWd/MT
     + dcday,     ! integer array(1000) dischange day 1-31
     + dcmonth,   ! integer array(1000) dischange month 1-12
     + dcyear,    ! integer array(1000) dischange year 1900-3000
     + icycle,    ! integer array(1000)
     + thresA,    ! real*4 array(1000) threshold for neutron channel A
     + thresB,    ! real*4 array(1000) threshold for neutron channel B
     + mday,      ! integer array(1000) measurement day 1-31
     + mmonth,    ! integer array(1000) measurement month 1-12
     + myear,     ! integer array(1000) measurement year 1900-3000
     + ct,        ! real*4 array(1000) declared cooling time in years
     + na,        ! real*4 array(1000) neutrons A
     + nb,        ! real*4 array(1000) neutrons B
     + nc,        ! real*4 array(1000) neutrons C
     + gamma1,    ! real*4 array(1000) gamma dose 1
     + gamma2,    ! real*4 array(1000) gamma dose 2
     + detector,  ! character*32 array(1000) detector ids

     + lognog,    ! real*4 array(1000) log10 of the N/G
     + logbu,     ! real*4 array(1000) log10 of the burnup
     + logct,     ! real*4 array(1000) log10 of the cooling time
     + logdcn,    ! real*4 array(1000) log10 of the decay corrected total neutrons
     + loggamma,  ! real*4 array(1000) log10 of the gamma dose

     + ierror     ! integer returned 0 if no errors, 1 on error

     + )

	!DEC$ ATTRIBUTES DLLEXPORT :: readmeasurementplan
     
	implicit none

c	begin input variables

	character mpfilename*280
	integer i
	character dbheader*200
	character facility*32(1000),id*32(1000)
	integer mtype(1000),mstatus(1000),icycle(1000)
	integer dcday(1000),dcmonth(1000),dcyear(1000)
	integer mday(1000),mmonth(1000),myear(1000)
	real*4 en(1000),bu(1000),thresA(1000),thresB(1000)
	real*4 ct(1000),na(1000),nb(1000),nc(1000)
	real*4 gamma1(1000),gamma2(1000)
	character detector*32(1000)

	real*4 lognog(1000)
	real*4 logbu(1000),logct(1000),logdcn(1000),loggamma(1000)

	integer ierror

c	end input variables

c	begin internal variable


	integer ic
	character string*200,f*32

c	end internal variable

	open(1,file=mpfilename,status='old')
	read(1,'(a200)') dbheader
	i=0
10	read(1,'(a200)',err=99,end=99) string
	i=i+1
	if (i.gt.1000) then
	 ierror=1
	 goto 100
	endif
	ic=1
	call getnextstring(string,ic,f)
	facility(i)=f
	call getnextstring(string,ic,f)
	id(i)=f
	call getnextstring(string,ic,f)
	call getintegerfromf(f,mtype(i))
	if (mtype(i).lt.0.or.mtype(i).gt.9) mtype(i)=0
	call getnextstring(string,ic,f)
	call getintegerfromf(f,mstatus(i))
	if (mstatus(i).lt.0.or.mstatus(i).gt.9) mstatus(i)=0
	call getnextstring(string,ic,f)
	call getreal8fromf(f,en(i))
	call getnextstring(string,ic,f)
	call getreal8fromf(f,bu(i))
	call getnextstring(string,ic,f)
	call getintegerfromf(f,dcday(i))
	call getnextstring(string,ic,f)
	call getintegerfromf(f,dcmonth(i))
	call getnextstring(string,ic,f)
	call getintegerfromf(f,dcyear(i))
	call getnextstring(string,ic,f)
	call getintegerfromf(f,icycle(i))
	call getnextstring(string,ic,f)
	call getreal8fromf(f,thresA(i))
	call getnextstring(string,ic,f)
	call getreal8fromf(f,thresB(i))
	call getnextstring(string,ic,f)
	call getintegerfromf(f,mday(i))
	call getnextstring(string,ic,f)
	call getintegerfromf(f,mmonth(i))
	call getnextstring(string,ic,f)
	call getintegerfromf(f,myear(i))
	call getnextstring(string,ic,f)
	call getreal8fromf(f,ct(i))
	call getnextstring(string,ic,f)
	call getreal8fromf(f,na(i))
	if (na(i).lt.0.01) na(i)=0.01
	call getnextstring(string,ic,f)
	call getreal8fromf(f,nb(i))
	if (nb(i).lt.0.01) nb(i)=0.01
	call getnextstring(string,ic,f)
	call getreal8fromf(f,nc(i))
	if (nc(i).lt.0.01) nc(i)=0.01
	call getnextstring(string,ic,f)
	call getreal8fromf(f,gamma1(i))
	if (gamma1(i).lt.0.0001) gamma1(i)=0.0001
	call getnextstring(string,ic,f)
	call getreal8fromf(f,gamma2(i))
	if (gamma2(i).lt.0.0001) gamma2(i)=0.0001
	call getnextstring(string,ic,f)
	detector(i)=f
	
	if (ct(i).eq.0) then
	 ct(i)=real(myear(i)-dcyear(i))+real(mmonth(i)-dcmonth(i))/12.
     +      +real(mday(i)-dcday(i))/365.
	 if (ct(i).lt.0.0) ct(i)=0.0
	endif
	if (mtype(i).eq.1) then
	 if (na(i).eq.0..and.nb(i).eq.0.) then
	  lognog(i)=-999.9
	 else
	  lognog(i)=log10((na(i)+nb(i))/gamma1(i))
	 endif
	 if (gamma1(i).eq.0.) then
	  loggamma(i)=-999.9
	 else
	  loggamma(i)=log10(gamma1(i))
	 endif
	else
	 if (na(i).eq.0..and.nb(i).eq.0.) then
	  logdcn(i)=-999.9
	 else
	  logdcn(i)=log10((na(i)+nb(i))*exp(0.03828*ct(i)))
	 endif
	 if (gamma1(i).eq.0.) then
	  loggamma(i)=-999.9
	 else
	  loggamma(i)=log10(gamma1(i))
	 endif
	 if (ct(i).eq.0.) then
	  logct(i)=-999.9
	 else
	  logct(i)=log10(ct(i))
	 endif
	 if (bu(i).eq.0.) then
	  logbu(i)=-999.9
	 else
	  logbu(i)=log10(bu(i))
	 endif
	endif

	goto 10

99	ierror=0
100	close(1)

	end

c	******************************************************************************