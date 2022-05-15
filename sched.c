#include "sched.h"
void FCFS(
		int number_of_jobs,
		const int job_submitted_time [],
		const int job_required_time [],
		int job_sched_start []
) {
		int timeNow = 0;
		int i,j;
		int min = 0;
		int minFlag = 0;
		int vis[2005] = {0};
		for (i = 0; i < number_of_jobs; i++) {
			min = 1000000 + 999;
			minFlag = -1;
			for (j = 0; j < number_of_jobs; j++) {
				if (vis[j]) {
					continue;
				}
				if (job_submitted_time[j] < min) {
					min = job_submitted_time[j];
					minFlag = j;
				}
			}
			vis[minFlag] = 1;
			if (timeNow < job_submitted_time[minFlag]) {
				job_sched_start[minFlag] = job_submitted_time[minFlag];
			} else {
				job_sched_start[minFlag] = timeNow;
			}
			timeNow = job_sched_start[minFlag] + job_required_time[minFlag];
		}
}

void SJF (
        int number_of_jobs,
        const int job_submitted_time [],
        const int job_required_time [],
        int job_sched_start []) {
		int timeNow = 0;
		int minAva = 1000000 + 999;
		int minFlagAva = -1;
		int minFutureSt = 1000000 + 999;
		int minFlagFuture = -1;
		int vis[2005] = {0};
		int i, j;
		int minFutureEd;
		for (i = 0; i < number_of_jobs; i++) {
			minAva = 3000000 + 999;
			minFutureSt = minAva;
			minFutureEd = minAva;
			minFlagAva = -1;
			minFlagFuture = -1;
			for (j = 0; j < number_of_jobs; j++) {
				if (vis[j]) {
					continue;
				}
				if (job_submitted_time[j] <= timeNow) {
					if (job_required_time[j] < minAva) {
						minAva = job_required_time[j];
						minFlagAva = j;
					} else if (job_required_time[j] == minAva && job_submitted_time[j] < job_submitted_time[minFlagAva]) {
						minFlagAva = j;
					} else if (job_required_time[j] == minAva && job_submitted_time[j] == job_submitted_time[minFlagAva] && j < minFlagAva) {
						minFlagAva = j;
					}
				} else {
					if (job_submitted_time[j] < minFutureSt) {
						minFutureSt = job_submitted_time[j];
						minFutureEd = job_submitted_time[j] + job_required_time[j];
						minFlagFuture = j;
					} else if (job_submitted_time[j] == minFutureSt) {
						if (job_required_time[j] < job_required_time[minFlagFuture]) {
							minFutureEd = minFutureSt + job_required_time[j];
							minFlagFuture  = j;
						} else if (job_required_time[j] == job_required_time[minFlagFuture]) {
							if (j < minFlagFuture) {
								minFlagFuture = j;
							}
						}
					}
				}
			}
			//vis[minFlagFuture] = 1;
			if (minFlagAva != -1) {
				vis[minFlagAva] = 1;
				job_sched_start[minFlagAva] = timeNow;
				timeNow = timeNow + job_required_time[minFlagAva];
			} else {
				vis[minFlagFuture] = 1;
				job_sched_start[minFlagFuture] = minFutureSt;
				timeNow = minFutureEd;
			}
	}
}	
