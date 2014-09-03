
	subroutine readdatabase(

c	outputs
     + dbfilename,! character*280
     + i,         ! integer number of entries in database.csv
     + dbheader,  ! character*200 database.csv file header
     + facility,  ! character*32 array(10000) facility names
     + id,        ! character*32 array(10000) item IDs
     + mtype,     ! integer array(10000) measurement type
                  ! 1 cycle verification
	            ! 2 assembly verification
	            ! 3 item reverification
     + mstatus,   ! integer array(10000)
     + en,        ! real*4 array(10000) 235U enrichment level in %
     + bu,        ! real*4 array(10000) declared burnup in GWd/MT
     + dcday,     ! integer array(10000) dischange day 1 to 31
     + dcmonth,   ! integer array(10000) dischange month 1 to 12
     + dcyear,    ! integer array(10000) dischange year 1900 to 3000
     + icycle,    ! integer array(10000)
     + thresA,    ! real*4 array(10000) threshold for neutron channel A
     + thresB,    ! real*4 array(10000) threshold for neutron channel B
     + mday,      ! integer array(10000) measurement day 1-31
     + mmonth,    ! integer array(10000) measurement month 1-12
     + myear,     ! integer array(10000) measurement year 1900-3000
     + ct,        ! real*4 array(10000) declared cooling time in years
     + na,        ! real*4 array(10000) neutrons A
     + nb,        ! real*4 array(10000) neutrons B
     + nc,        ! real*4 array(10000) neutrons C
     + gamma1,    ! real*4 array(10000) gamma dose 1
     + gamma2,    ! real*4 array(10000) gamma dose 2
     + detector,  ! character*32 array(10000) detector names

     + lognog,    ! real*4 array(10000) log10 of N/G
     + logbu,     ! real*4 array(10000) log10 of the burnup
     + logct,     ! real*4 array(10000) log10 of the cooling time
     + logdcn,    ! real*4 array(10000) log10 of the decay corrected total neutrons
     + loggamma,  ! real*4 array(10000) log10 of the gamma dose

     + ierror     ! integer returned 0 if no errors, 1 on error

     + )
     
	!DEC$ ATTRIBUTES DLLEXPORT :: readdatabase

	implicit none

c	begin input variables

	character dbfilename*280
	integer i
	character dbheader*200
	character facility*32(10000),id*32(10000)
	integer mtype(10000),mstatus(10000),icycle(10000)
	integer dcday(10000),dcmonth(10000),dcyear(10000)
	integer mday(10000),mmonth(10000),myear(10000)
	real*4 en(10000),bu(10000),thresA(10000),thresB(10000)
	real*4 ct(10000),na(10000),nb(10000),nc(10000)
	real*4 gamma1(10000),gamma2(10000)
	character detector*32(10000)

	real*4 lognog(10000)
	real*4 logbu(10000),logct(10000),logdcn(10000),loggamma(10000)

	integer ierror

c	end input variables

c	begin internal variable

	integer ic
	character string*200,f*32

c	end internal variable

	open(1,file=dbfilename,status='old')
	read(1,'(a200)') dbheader
	i=0
10	read(1,'(a200)',err=99,end=99) string
	i=i+1
	if (i.gt.10000) then
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

	subroutine writedatabase(

c	input
     + dbfilename,! character*280
     + ii,        ! integer number of entries in database.csv
     + dbheader,  ! character*200 database.csv file header
     + facility,  ! character*32 array(10000) facility names
     + id,        ! character*32 array(10000) item IDs
     + mtype,     ! integer array(10000) measurement type
                  ! 1 cycle verification
	            ! 2 assembly verification
	            ! 3 item reverification
     + mstatus,   ! integer array(10000)
     + en,        ! real*4 array(10000) 235U enrichment level in %
     + bu,        ! real*4 array(10000) declared burnup in GWd/MT
     + dcday,     ! integer array(10000) dischange day 1-31
     + dcmonth,   ! integer array(10000) dischange month 1-12
     + dcyear,    ! integer array(10000) dischange year 1900-3000
     + icycle,    ! integer array(10000)
     + thresA,    ! real*4 array(10000) threshold for neutron channel A
     + thresB,    ! real*4 array(10000) threshold for neutron channel B
     + mday,      ! integer array(10000) measurement day 1-31
     + mmonth,    ! integer array(10000) measurement month 1-12
     + myear,     ! integer array(10000) measurement year 1900-3000
     + ct,        ! real*4 array(10000) declared cooling time in years
     + na,        ! real*4 array(10000) neutrons A
     + nb,        ! real*4 array(10000) neutrons B
     + nc,        ! real*4 array(10000) neutrons C
     + gamma1,    ! real*4 array(10000) gamma dose 1
     + gamma2,    ! real*4 array(10000) gamma dose 2
     + detector   ! character*32 array(10000) detector names

     + )
     
	!DEC$ ATTRIBUTES DLLEXPORT :: writedatabase

	implicit none

c	begin input variables

	character dbfilename*280
	integer ii
	character dbheader*200
	character facility*32(10000),id*32(10000)
	integer mtype(10000),mstatus(10000),icycle(10000)
	integer dcday(10000),dcmonth(10000),dcyear(10000)
	integer mday(10000),mmonth(10000),myear(10000)
	real*4 en(10000),bu(10000),thresA(10000),thresB(10000)
	real*4 ct(10000),na(10000),nb(10000),nc(10000)
	real*4 gamma1(10000),gamma2(10000)
	character detector*32(10000)

c	end input variables

c	begin internal variable

	integer i
	character string*200,f*32

c	end internal variable


	open(1,file=dbfilename,status='unknown')
1	write(1,'(a200)') dbheader
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

	subroutine getreal8fromf(f,num)
	implicit none
	character f*32,string*31
	real*4 num
10	if (f(1:1).eq.'	') then
	 string=f(2:32)
	 f=string//' '
	 goto 10
	endif
	if (f.eq.'                              ') then
	 num=0.
	else
	 read(f,*,err=99,end=99) num
	 return
	endif
99	num=0.
	end

c	******************************************************************************

	subroutine getintegerfromf(f,num)
	implicit none
	character f*32,string*31
	integer num
10	if (f(1:1).eq.'	') then
	 string=f(2:32)
	 f=string//' '
	 goto 10
	endif
	if (f.eq.'                              ') then
	 num=0
	else
	 read(f,*,err=99,end=99) num
	 return
	endif
99	num=0
	end

c	******************************************************************************

	subroutine getnextstring(string,ic,f)
	implicit none

	integer ic,i,j
	character string*200,f*32

	i=ic
	if (ic.gt.200) then
	 f(1:32)='                                '
	 return
	endif
10	if (i.le.200) then
	 if (string(i:i).eq.',') then
	  do j=i-ic+1,32
	   f(j:j)=' '
	  enddo
        ic=i+1
	 else
	  if (i-ic+1.le.32) f(i-ic+1:i-ic+1)=string(i:i)
	  i=i+1
	  goto 10
	 endif
	else
	 ic=201
	endif

	end

c	******************************************************************************

	subroutine turn_db_into_mp(

c	input
     + ii,        ! integer number of entries in database.csv
     + dbheader,  ! character*200 database.csv file header
     + facility,  ! character*32 array(10000) facility names
     + id,        ! character*32 array(10000) item IDs
     + mtype,     ! integer array(10000) measurement type
                  ! 1 cycle verification
	            ! 2 assembly verification
	            ! 3 item reverification
     + en,        ! real*4 array(10000) 235U enrichment level in %
     + bu,        ! real*4 array(10000) declared burnup in GWd/MT
     + dcday,     ! integer array(10000) dischange day 1-31
     + dcmonth,   ! integer array(10000) dischange month 1-12
     + dcyear,    ! integer array(10000) dischange year 1900-3000
     + icycle,    ! integer array(10000)
     + thresA,    ! real*4 array(10000) threshold for neutron channel A
     + thresB,    ! real*4 array(10000) threshold for neutron channel B
     + mday,      ! integer array(10000) measurement day 1-31
     + mmonth,    ! integer array(10000) measurement month 1-21
     + myear,     ! integer array(10000) measurement year 1900-3000
     + ct,        ! real*4 array(10000) declared cooling time in years
     + na,        ! real*4 array(10000) neutrons A
     + nb,        ! real*4 array(10000) neutrons B
     + nc,        ! real*4 array(10000) neutrons C
     + gamma1,    ! real*4 array(10000) gamma dose 1
     + gamma2,    ! real*4 array(10000) gamma dose 2
     + detector   ! character*32 array(10000) detector names

     + )
     
	implicit none

c	begin input variables

	integer ii
	character dbheader*200
	character facility*32(10000),id*32(10000)
	integer mtype(10000),icycle(10000)
	integer dcday(10000),dcmonth(10000),dcyear(10000)
	integer mday(10000),mmonth(10000),myear(10000)
	real*4 en(10000),bu(10000),thresA(10000),thresB(10000)
	real*4 ct(10000),na(10000),nb(10000),nc(10000)
	real*4 gamma1(10000),gamma2(10000)
	character detector*32(10000)

c	end input variables

c	begin internal variable

	integer i
	character string*200,f*32

c	end internal variable

	open(1,file='measurementplan.csv',status='unknown')
	write(1,'(a200)') dbheader
	do i=1,ii
	 string(1:20)=facility(i)
	 string(21:21)=','
	 string(22:41)=id(i)
	 string(42:42)=','
	 write(string(43:43),'(i1)') mtype(i)
	 string(44:46)=',0,'
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
