/******************************************************************************
 *  bwm-ng output                                                             *
 *                                                                            *
 *  Copyright (C) 2004 Volker Gropp (vgropp@pefra.de)                         *
 *                                                                            *
 *  for more info read README.                                                *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the Free Software               *
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA *
 *                                                                            *
 *****************************************************************************/

#include "output.h"


/* convert doube bytes/s value to some nice string */
inline char *convert_bytes(double bytes,char * buffer, int buf_size) {
    if (bytes<0) bytes=0;
    if (bytes<1024 && !show_kb)
        snprintf(buffer,buf_size,"%12.2f  B/s",bytes);
    else if (bytes<1048576 || show_kb)
        snprintf(buffer,buf_size,"%12.2f KB/s",bytes/1024);
    else snprintf(buffer,buf_size,"%12.2f MB/s",bytes/1048576);
    return buffer;
}

int print_header(int option) {
#if HTML
    FILE *tmp_out_file;
#endif    
	switch (output_method) {
#ifdef HAVE_CURSES
		case CURSES_OUT:
	        erase();
		    mvwprintw(stdscr,1,8,"bwm-ng v%i.%i%s (probing every %2.3fs), press 'q' to end this",MAJOR,MINOR,EXTRA,(float)delay/1000);
			switch (input_method) {
#ifdef LIBSTATGRAB
				case LIBSTAT_IN:
					mvwprintw(stdscr,2,8,"libstatgrab");
					break;
#endif
#ifdef GETIFADDRS
				case GETIFADDRS_IN:
			        mvwprintw(stdscr,2,8,"getifaddrs");
					break;
#endif
#ifdef PROC_NET_DEV			
				case PROC_IN:
					mvwprintw(stdscr,2,8,"/proc/net/dev");
					break;
#endif
#ifdef NETSTAT
				case NETSTAT_IN:
					mvwprintw(stdscr,2,8,"netstat -i");
					break;
#endif			
#ifdef SYSCTL
				case SYSCTL_IN:
					mvwprintw(stdscr,2,8,"sysctl");
					break;
#endif					
#if HAVE_LIBKSTAT
                case KSTAT_IN:
                    mvwprintw(stdscr,2,8,"kstat");
                    break;
#endif
			}
            switch (show_all_if) {
                case 1:
                     wprintw(stdscr," %s","(all)");
                    break;
                case 2:
                     wprintw(stdscr," %s","(all and down)");
                    break;
            }
	        mvwprintw(stdscr,3,8,"%c       iface               Rx                Tx             Total",(char)IDLE_CHARS[option]);
	        /* go to next char for next run */
	        option++;
			if (option>3) option=0;
	        mvwprintw(stdscr,4,8,"==================================================================");
			break;
#endif
#ifdef HTML
		case HTML_OUT:
            tmp_out_file=out_file==NULL ? stdout : out_file;
			if (html_header) {
		        fprintf(tmp_out_file,"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n<html>\n<head>\n");
		        fprintf(tmp_out_file,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n");
		        fprintf(tmp_out_file,"<META HTTP-EQUIV=Refresh CONTENT=\"%i\">\n",html_refresh);
		        fprintf(tmp_out_file,"<link rel=\"stylesheet\" href=\"bwm-ng.css\" type=\"text/css\" media=\"screen,projection,print\">\n");
		        fprintf(tmp_out_file,"<title>bwm-ng stats</title>\n</head>\n<body>\n");
			}
	        fprintf(tmp_out_file,"<div class='bwm-ng-header'>bwm-ng bwm-ng v%i.%i%s (refresh %is); input: ",MAJOR,MINOR,EXTRA,html_refresh);
			switch (input_method) {
#ifdef SYSCTL
				case SYSCTL_IN:
					fprintf(tmp_out_file,"sysctl");
					break;
#endif				
#ifdef NETSTAT				
				case NETSTAT_IN:
					fprintf(tmp_out_file,"netstat -i");
					break;
#endif					
#ifdef LIBSTATGRAB
				case LIBSTAT_IN:
					fprintf(tmp_out_file,"libstatgrab");
					break;
#endif
#ifdef GETIFADDRS
				case GETIFADDRS_IN:
					fprintf(tmp_out_file,"getifaddrs");
					break;
#endif
#ifdef PROC_NET_DEV
				case PROC_IN:
					fprintf(tmp_out_file,"/proc/net/dev");
					break;
#endif
#if HAVE_LIBKSTAT
                case KSTAT_IN:
                    fprintf(tmp_out_file,"kstat");
                    break;
#endif
			}
            switch (show_all_if) {
                case 1:
                     fprintf(tmp_out_file," %s","(all)");
                    break;
                case 2:
                     fprintf(tmp_out_file," %s","(all and down)");
                    break;
            }
	        fprintf(tmp_out_file,"</div><table class='bwm-ng-output'>");
			fprintf(tmp_out_file,"<tr class='bwm-ng-head'><td class='bwm-ng-name'>Interface</td><td>Rx</td><td>Tx</td><td>Total</td></tr>");
			break;
#endif			
		case PLAIN_OUT_ONCE:
		case PLAIN_OUT:
			if (output_method==PLAIN_OUT) printf("\033[1;2H");
	        printf("bwm-ng v%i.%i%s (delay %2.3fs); ",MAJOR,MINOR,EXTRA,(float)delay/1000);
			if (output_method==PLAIN_OUT) printf("press 'ctrl-c' to end this\033[2;2H"); else printf("input: ");
            switch (input_method) {
#ifdef SYSCTL
                case SYSCTL_IN:
                    printf("sysctl");
                    break;
#endif
#ifdef NETSTAT
                case NETSTAT_IN:
                    printf("netstat -i");
                    break;
#endif
#ifdef LIBSTATGRAB
                case LIBSTAT_IN:
                    printf("libstatgrab");
                    break;
#endif
#ifdef GETIFADDRS
                case GETIFADDRS_IN:
                    printf("getifaddrs");
                    break;
#endif
#ifdef PROC_NET_DEV
                case PROC_IN:
                    printf("/proc/net/dev");
                    break;
#endif
#if HAVE_LIBKSTAT
                case KSTAT_IN:
                    printf("kstat");
                    break;
#endif
            }
            switch (show_all_if) {
                case 0:
                    printf("\n");
                    break;
                case 1:
                    printf(" %s\n","(all)");
                    break;
                case 2:
                    printf(" %s\n","(all and down)");
                    break;
            }
			if (output_method==PLAIN_OUT) {
				printf("\033[3;2H");
				printf("%c",(char)IDLE_CHARS[option]); 
			} else printf(" ");
			printf("       iface               Rx                Tx             Total\n");
			if (output_method==PLAIN_OUT) printf("\033[4;2H");
	        printf("==================================================================\n");
			/* go to next char for next run */
			option++;
			if (option>3) option=0;
			break;
	}
	return option;
}


inline unsigned long long calc_new_values(unsigned long long new, unsigned long long old) {
    /* FIXME: WRAP_AROUND _might_ be wrong for libstatgrab, where the type is always long long */
    return ((unsigned long long)(new-old) >= 0) ? (unsigned long long)(new-old) : (unsigned long long)((
#ifdef HAVE_LIBKSTAT
            (input_method==KSTAT_IN) ?
            WRAP_32BIT :
#endif            
            WRAP_AROUND)
            -old)+new;
}

/* do the actual output */
void print_values(int y,int x,char *if_name,t_iface_stats new_stats,t_iface_stats stats,float multiplier) {
	unsigned long long packets_out,packets_in,bytess,bytesr,errors_in,errors_out;
#if CSV || HTML
	FILE *tmp_out_file;
#endif
	errors_in=calc_new_values(new_stats.e_rec,stats.e_rec);
	errors_out=calc_new_values(new_stats.e_send,stats.e_send);
    packets_out=calc_new_values(new_stats.p_send,stats.p_send);
    packets_in=calc_new_values(new_stats.p_rec,stats.p_rec);
    bytess=calc_new_values(new_stats.send,stats.send);
    bytesr=calc_new_values(new_stats.rec,stats.rec);
    switch (output_method) {
#ifdef HAVE_CURSES		
        case CURSES_OUT:
            mvwprintw(stdscr,y,x,"%12s:",if_name); /* output the name */
            if (
#if !NETSTAT_BSD_BYTES && !NETSTAT_NETBSD && NETSTAT
					(input_method==NETSTAT_IN) || 
#endif					
					show_packets) {
                /* show packets/s if asked for or netstat input */
				if (errors_in) wattron(stdscr, A_REVERSE);
                wprintw(stdscr,"%13.2f P/s",(double)packets_in*multiplier);
				if (errors_in) wattroff(stdscr, A_REVERSE);
				wprintw(stdscr," ");
				if (errors_out) wattron(stdscr, A_REVERSE);
                wprintw(stdscr,"%13.2f P/s",(double)packets_out*multiplier);
				if (errors_out) wattroff(stdscr, A_REVERSE);
				wprintw(stdscr," ");
				if (errors_out || errors_in) wattron(stdscr, A_REVERSE);
                wprintw(stdscr,"%13.2f P/s",(double)(packets_out+packets_in)*multiplier);
				if (errors_out || errors_in) wattroff(stdscr, A_REVERSE);
            } else {
                char bytes_buf[20];
                /* output Bytes/s */
				if (errors_in) wattron(stdscr, A_REVERSE);
                wprintw(stdscr,"%s",convert_bytes((double)(bytesr*multiplier),bytes_buf,20));
				if (errors_in) wattroff(stdscr, A_REVERSE);
				wprintw(stdscr," ");
				if (errors_out) wattron(stdscr, A_REVERSE);
                wprintw(stdscr,"%s",convert_bytes((double)(bytess*multiplier),bytes_buf,20));
				if (errors_out) wattroff(stdscr, A_REVERSE);
				wprintw(stdscr," ");
                /* print total (send+rec) of current iface */
				if (errors_out || errors_in) wattron(stdscr, A_REVERSE);
                wprintw(stdscr,"%s",convert_bytes((double)((bytess+bytesr)*multiplier),bytes_buf,20));
				if (errors_out || errors_in) wattroff(stdscr, A_REVERSE);
            }
            break;
#endif
		case PLAIN_OUT_ONCE:
        case PLAIN_OUT:
			if (output_method==PLAIN_OUT) printf("\033[%d;2H",y);
            printf("%12s:",if_name); /* output the name */
            if (
#if !NETSTAT_BSD_BYTES && !NETSTAT_NETBSD && NETSTAT
					(input_method==NETSTAT_IN) || 
#endif					
					show_packets) {
                /* show packets/s if asked for or netstat input */
                printf("%13.2f P/s ",(double)packets_in*multiplier);
                printf("%13.2f P/s ",(double)packets_out*multiplier);
                printf("%13.2f P/s\n",(double)(packets_out+packets_in)*multiplier);
            } else {
                char bytes_buf[20];
                /* output Bytes/s */
                printf("%s ",convert_bytes((double)(bytesr*multiplier),bytes_buf,20));
                printf("%s ",convert_bytes((double)(bytess*multiplier),bytes_buf,20));
                /* print total (send+rec) of current iface */
                printf("%s\n",convert_bytes((double)((bytess+bytesr)*multiplier),bytes_buf,20));
            }
            break;
#ifdef HTML			
		case HTML_OUT:
            tmp_out_file=out_file==NULL ? stdout : out_file;
			fprintf(tmp_out_file,"<tr><td class='bwm-ng-name'>%12s:</td>",if_name);
			if (
#if !NETSTAT_BSD_BYTES && !NETSTAT_NETBSD && NETSTAT
					(input_method==NETSTAT_IN) || 
#endif					
					show_packets) {
				/* show packets/s if asked for or netstat input */
                fprintf(tmp_out_file,"<td class='bwm-ng-out'>");
				if (errors_in) fprintf(tmp_out_file,"<span class='bwm-ng-error'>"); else fprintf(tmp_out_file,"<span class='bwm-ng-dummy'>");
				fprintf(tmp_out_file,"%13.2f P/s</span> </td>",(double)packets_in*multiplier);
                fprintf(tmp_out_file,"<td class='bwm-ng-in'>");
				if (errors_out) fprintf(tmp_out_file,"<span class='bwm-ng-error'>"); else fprintf(tmp_out_file,"<span class='bwm-ng-dummy'>");
				fprintf(tmp_out_file,"%13.2f P/s</span> </td>",(double)packets_out*multiplier);
                fprintf(tmp_out_file,"<td class='bwm-ng-total'>");
				if (errors_out || errors_in) 
					fprintf(tmp_out_file,"<span class='bwm-ng-error'>"); 
				else 
					fprintf(tmp_out_file,"<span class='bwm-ng-dummy'>");
				fprintf(tmp_out_file,"%13.2f P/s</span></td><tr>\n",(double)(packets_out+packets_in)*multiplier);
            } else {
                char bytes_buf[20];
                /* output Bytes/s */
                fprintf(tmp_out_file,"<td class='bwm-ng-out'>");
				if (errors_in) fprintf(tmp_out_file,"<span class='bwm-ng-error'>"); else fprintf(tmp_out_file,"<span class='bwm-ng-dummy'>");
				fprintf(tmp_out_file,"%s</span> </td>",convert_bytes((double)(bytesr*multiplier),bytes_buf,20));
                fprintf(tmp_out_file,"<td class='bwm-ng-in'>");
				if (errors_out) fprintf(out_file,"<span class='bwm-ng-error'>"); else fprintf(tmp_out_file,"<span class='bwm-ng-dummy'>");
				fprintf(tmp_out_file,"%s</span> </td>",convert_bytes((double)(bytess*multiplier),bytes_buf,20));
                /* print total (send+rec) of current iface */
                fprintf(tmp_out_file,"<td class='bwm-ng-total'>");
				if (errors_out || errors_in) 
					fprintf(tmp_out_file,"<span class='bwm-ng-errors'>"); 
				else 
					fprintf(tmp_out_file,"<span class='bwm-ng-dummy'>");
				fprintf(tmp_out_file,"%s</span></td></tr>\n",convert_bytes((double)((bytess+bytesr)*multiplier),bytes_buf,20));
            }
            break;
#endif
#ifdef CSV
        case CSV_OUT:
			tmp_out_file=out_file==NULL ? stdout : out_file;
            fprintf(tmp_out_file,"%i%c%s%c",(int)time(NULL),csv_char,if_name,csv_char);
            if (
#if !NETSTAT_BSD_BYTES && !NETSTAT_NETBSD && NETSTAT
					!(input_method==NETSTAT_IN) && 
#endif					
					!show_packets) {
                /* output Bytes/s */
                fprintf(tmp_out_file,"%.2f%c%.2f%c%.2f%c",(double)(bytess*multiplier),csv_char,(double)(bytesr*multiplier),csv_char,(double)((bytess+bytesr)*multiplier),csv_char);
            }
            /* show packets/s if asked for or netstat input */
            fprintf(tmp_out_file,"%.2f%c%.2f%c%.2f%c%llu%c%llu\n",(double)packets_out*multiplier,csv_char,(double)packets_in*multiplier,csv_char,(double)(packets_out+packets_in)*multiplier,csv_char,errors_out,csv_char,errors_in);
            break;
#endif			
    }
}

