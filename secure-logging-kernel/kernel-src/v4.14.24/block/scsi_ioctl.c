extern int kernel_init_done;
int printk(const char *fmt, ...);
/*
 * Copyright (C) 2001 Jens Axboe <axboe@suse.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public Licens
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-
 *
 */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/capability.h>
#include <linux/completion.h>
#include <linux/cdrom.h>
#include <linux/ratelimit.h>
#include <linux/slab.h>
#include <linux/times.h>
#include <linux/uio.h>
#include <linux/uaccess.h>

#include <scsi/scsi.h>
#include <scsi/scsi_ioctl.h>
#include <scsi/scsi_cmnd.h>

struct blk_cmd_filter {
	unsigned long read_ok[BLK_SCSI_CMD_PER_LONG];
	unsigned long write_ok[BLK_SCSI_CMD_PER_LONG];
};

static struct blk_cmd_filter blk_default_cmd_filter;

/* Command group 3 is reserved and should never be used.  */
const unsigned char scsi_command_size_tbl[8] =
{
	6, 10, 10, 12,
	16, 12, 10, 10
};
EXPORT_SYMBOL(scsi_command_size_tbl);

#include <scsi/sg.h>

static int sg_get_version(int __user *p)
{
	static const int sg_version_num = 30527;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return put_user(sg_version_num, p);
}

static int scsi_get_idlun(struct request_queue *q, int __user *p)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return put_user(0, p);
}

static int scsi_get_bus(struct request_queue *q, int __user *p)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return put_user(0, p);
}

static int sg_get_timeout(struct request_queue *q)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return jiffies_to_clock_t(q->sg_timeout);
}

static int sg_set_timeout(struct request_queue *q, int __user *p)
{
	int timeout, err = get_user(timeout, p);

	if (!err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		q->sg_timeout = clock_t_to_jiffies(timeout);
}

	return err;
}

static int max_sectors_bytes(struct request_queue *q)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	unsigned int max_sectors = queue_max_sectors(q);

	max_sectors = min_t(unsigned int, max_sectors, INT_MAX >> 9);

	return max_sectors << 9;
}

static int sg_get_reserved_size(struct request_queue *q, int __user *p)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	int val = min_t(int, q->sg_reserved_size, max_sectors_bytes(q));

	return put_user(val, p);
}

static int sg_set_reserved_size(struct request_queue *q, int __user *p)
{
	int size, err = get_user(size, p);

	if (err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return err;
}

	if (size < 0)
		return -EINVAL;

	q->sg_reserved_size = min(size, max_sectors_bytes(q));
	return 0;
}

/*
 * will always return that we are ATAPI even for a real SCSI drive, I'm not
 * so sure this is worth doing anything about (why would you care??)
 */
static int sg_emulated_host(struct request_queue *q, int __user *p)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return put_user(1, p);
}

static void blk_set_cmd_filter_defaults(struct blk_cmd_filter *filter)
{
	/* Basic read-only commands */
	__set_bit(TEST_UNIT_READY, filter->read_ok);
	__set_bit(REQUEST_SENSE, filter->read_ok);
	__set_bit(READ_6, filter->read_ok);
	__set_bit(READ_10, filter->read_ok);
	__set_bit(READ_12, filter->read_ok);
	__set_bit(READ_16, filter->read_ok);
	__set_bit(READ_BUFFER, filter->read_ok);
	__set_bit(READ_DEFECT_DATA, filter->read_ok);
	__set_bit(READ_CAPACITY, filter->read_ok);
	__set_bit(READ_LONG, filter->read_ok);
	__set_bit(INQUIRY, filter->read_ok);
	__set_bit(MODE_SENSE, filter->read_ok);
	__set_bit(MODE_SENSE_10, filter->read_ok);
	__set_bit(LOG_SENSE, filter->read_ok);
	__set_bit(START_STOP, filter->read_ok);
	__set_bit(GPCMD_VERIFY_10, filter->read_ok);
	__set_bit(VERIFY_16, filter->read_ok);
	__set_bit(REPORT_LUNS, filter->read_ok);
	__set_bit(SERVICE_ACTION_IN_16, filter->read_ok);
	__set_bit(RECEIVE_DIAGNOSTIC, filter->read_ok);
	__set_bit(MAINTENANCE_IN, filter->read_ok);
	__set_bit(GPCMD_READ_BUFFER_CAPACITY, filter->read_ok);

	/* Audio CD commands */
	__set_bit(GPCMD_PLAY_CD, filter->read_ok);
	__set_bit(GPCMD_PLAY_AUDIO_10, filter->read_ok);
	__set_bit(GPCMD_PLAY_AUDIO_MSF, filter->read_ok);
	__set_bit(GPCMD_PLAY_AUDIO_TI, filter->read_ok);
	__set_bit(GPCMD_PAUSE_RESUME, filter->read_ok);

	/* CD/DVD data reading */
	__set_bit(GPCMD_READ_CD, filter->read_ok);
	__set_bit(GPCMD_READ_CD_MSF, filter->read_ok);
	__set_bit(GPCMD_READ_DISC_INFO, filter->read_ok);
	__set_bit(GPCMD_READ_CDVD_CAPACITY, filter->read_ok);
	__set_bit(GPCMD_READ_DVD_STRUCTURE, filter->read_ok);
	__set_bit(GPCMD_READ_HEADER, filter->read_ok);
	__set_bit(GPCMD_READ_TRACK_RZONE_INFO, filter->read_ok);
	__set_bit(GPCMD_READ_SUBCHANNEL, filter->read_ok);
	__set_bit(GPCMD_READ_TOC_PMA_ATIP, filter->read_ok);
	__set_bit(GPCMD_REPORT_KEY, filter->read_ok);
	__set_bit(GPCMD_SCAN, filter->read_ok);
	__set_bit(GPCMD_GET_CONFIGURATION, filter->read_ok);
	__set_bit(GPCMD_READ_FORMAT_CAPACITIES, filter->read_ok);
	__set_bit(GPCMD_GET_EVENT_STATUS_NOTIFICATION, filter->read_ok);
	__set_bit(GPCMD_GET_PERFORMANCE, filter->read_ok);
	__set_bit(GPCMD_SEEK, filter->read_ok);
	__set_bit(GPCMD_STOP_PLAY_SCAN, filter->read_ok);

	/* Basic writing commands */
	__set_bit(WRITE_6, filter->write_ok);
	__set_bit(WRITE_10, filter->write_ok);
	__set_bit(WRITE_VERIFY, filter->write_ok);
	__set_bit(WRITE_12, filter->write_ok);
	__set_bit(WRITE_VERIFY_12, filter->write_ok);
	__set_bit(WRITE_16, filter->write_ok);
	__set_bit(WRITE_LONG, filter->write_ok);
	__set_bit(WRITE_LONG_2, filter->write_ok);
	__set_bit(WRITE_SAME, filter->write_ok);
	__set_bit(WRITE_SAME_16, filter->write_ok);
	__set_bit(WRITE_SAME_32, filter->write_ok);
	__set_bit(ERASE, filter->write_ok);
	__set_bit(GPCMD_MODE_SELECT_10, filter->write_ok);
	__set_bit(MODE_SELECT, filter->write_ok);
	__set_bit(LOG_SELECT, filter->write_ok);
	__set_bit(GPCMD_BLANK, filter->write_ok);
	__set_bit(GPCMD_CLOSE_TRACK, filter->write_ok);
	__set_bit(GPCMD_FLUSH_CACHE, filter->write_ok);
	__set_bit(GPCMD_FORMAT_UNIT, filter->write_ok);
	__set_bit(GPCMD_REPAIR_RZONE_TRACK, filter->write_ok);
	__set_bit(GPCMD_RESERVE_RZONE_TRACK, filter->write_ok);
	__set_bit(GPCMD_SEND_DVD_STRUCTURE, filter->write_ok);
	__set_bit(GPCMD_SEND_EVENT, filter->write_ok);
	__set_bit(GPCMD_SEND_KEY, filter->write_ok);
	__set_bit(GPCMD_SEND_OPC, filter->write_ok);
	__set_bit(GPCMD_SEND_CUE_SHEET, filter->write_ok);
	__set_bit(GPCMD_SET_SPEED, filter->write_ok);
	__set_bit(GPCMD_PREVENT_ALLOW_MEDIUM_REMOVAL, filter->write_ok);
	__set_bit(GPCMD_LOAD_UNLOAD, filter->write_ok);
	__set_bit(GPCMD_SET_STREAMING, filter->write_ok);
	__set_bit(GPCMD_SET_READ_AHEAD, filter->write_ok);
}

int blk_verify_command(unsigned char *cmd, fmode_t has_write_perm)
{
	struct blk_cmd_filter *filter = &blk_default_cmd_filter;

	/* root can do any command. */
	if (capable(CAP_SYS_RAWIO))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 0;
}

	/* Anybody who can open the device can do a read-safe command */
	if (test_bit(cmd[0], filter->read_ok))
		return 0;

	/* Write-safe commands require a writable open */
	if (test_bit(cmd[0], filter->write_ok) && has_write_perm)
		return 0;

	return -EPERM;
}
EXPORT_SYMBOL(blk_verify_command);

static int blk_fill_sghdr_rq(struct request_queue *q, struct request *rq,
			     struct sg_io_hdr *hdr, fmode_t mode)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct scsi_request *req = scsi_req(rq);

	if (copy_from_user(req->cmd, hdr->cmdp, hdr->cmd_len))
		return -EFAULT;
	if (blk_verify_command(req->cmd, mode & FMODE_WRITE))
		return -EPERM;

	/*
	 * fill in request structure
	 */
	req->cmd_len = hdr->cmd_len;

	rq->timeout = msecs_to_jiffies(hdr->timeout);
	if (!rq->timeout)
		rq->timeout = q->sg_timeout;
	if (!rq->timeout)
		rq->timeout = BLK_DEFAULT_SG_TIMEOUT;
	if (rq->timeout < BLK_MIN_SG_TIMEOUT)
		rq->timeout = BLK_MIN_SG_TIMEOUT;

	return 0;
}

static int blk_complete_sghdr_rq(struct request *rq, struct sg_io_hdr *hdr,
				 struct bio *bio)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct scsi_request *req = scsi_req(rq);
	int r, ret = 0;

	/*
	 * fill in all the output members
	 */
	hdr->status = req->result & 0xff;
	hdr->masked_status = status_byte(req->result);
	hdr->msg_status = msg_byte(req->result);
	hdr->host_status = host_byte(req->result);
	hdr->driver_status = driver_byte(req->result);
	hdr->info = 0;
	if (hdr->masked_status || hdr->host_status || hdr->driver_status)
		hdr->info |= SG_INFO_CHECK;
	hdr->resid = req->resid_len;
	hdr->sb_len_wr = 0;

	if (req->sense_len && hdr->sbp) {
		int len = min((unsigned int) hdr->mx_sb_len, req->sense_len);

		if (!copy_to_user(hdr->sbp, req->sense, len))
			hdr->sb_len_wr = len;
		else
			ret = -EFAULT;
	}

	r = blk_rq_unmap_user(bio);
	if (!ret)
		ret = r;

	return ret;
}

static int sg_io(struct request_queue *q, struct gendisk *bd_disk,
		struct sg_io_hdr *hdr, fmode_t mode)
{
	unsigned long start_time;
	ssize_t ret = 0;
	int writing = 0;
	int at_head = 0;
	struct request *rq;
	struct scsi_request *req;
	struct bio *bio;

	if (hdr->interface_id != 'S')
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

	if (hdr->dxfer_len > (queue_max_hw_sectors(q) << 9))
		return -EIO;

	if (hdr->dxfer_len)
		switch (hdr->dxfer_direction) {
		default:
			return -EINVAL;
		case SG_DXFER_TO_DEV:
			writing = 1;
			break;
		case SG_DXFER_TO_FROM_DEV:
		case SG_DXFER_FROM_DEV:
			break;
		}
	if (hdr->flags & SG_FLAG_Q_AT_HEAD)
		at_head = 1;

	ret = -ENOMEM;
	rq = blk_get_request(q, writing ? REQ_OP_SCSI_OUT : REQ_OP_SCSI_IN,
			GFP_KERNEL);
	if (IS_ERR(rq))
		return PTR_ERR(rq);
	req = scsi_req(rq);

	if (hdr->cmd_len > BLK_MAX_CDB) {
		req->cmd = kzalloc(hdr->cmd_len, GFP_KERNEL);
		if (!req->cmd)
			goto out_put_request;
	}

	ret = blk_fill_sghdr_rq(q, rq, hdr, mode);
	if (ret < 0)
		goto out_free_cdb;

	ret = 0;
	if (hdr->iovec_count) {
		struct iov_iter i;
		struct iovec *iov = NULL;

		ret = import_iovec(rq_data_dir(rq),
				   hdr->dxferp, hdr->iovec_count,
				   0, &iov, &i);
		if (ret < 0)
			goto out_free_cdb;

		/* SG_IO howto says that the shorter of the two wins */
		iov_iter_truncate(&i, hdr->dxfer_len);

		ret = blk_rq_map_user_iov(q, rq, NULL, &i, GFP_KERNEL);
		kfree(iov);
	} else if (hdr->dxfer_len)
		ret = blk_rq_map_user(q, rq, NULL, hdr->dxferp, hdr->dxfer_len,
				      GFP_KERNEL);

	if (ret)
		goto out_free_cdb;

	bio = rq->bio;
	req->retries = 0;

	start_time = jiffies;

	/* ignore return value. All information is passed back to caller
	 * (if he doesn't check that is his problem).
	 * N.B. a non-zero SCSI status is _not_ necessarily an error.
	 */
	blk_execute_rq(q, bd_disk, rq, at_head);

	hdr->duration = jiffies_to_msecs(jiffies - start_time);

	ret = blk_complete_sghdr_rq(rq, hdr, bio);

out_free_cdb:
	scsi_req_free_cmd(req);
out_put_request:
	blk_put_request(rq);
	return ret;
}

/**
 * sg_scsi_ioctl  --  handle deprecated SCSI_IOCTL_SEND_COMMAND ioctl
 * @file:	file this ioctl operates on (optional)
 * @q:		request queue to send scsi commands down
 * @disk:	gendisk to operate on (option)
 * @sic:	userspace structure describing the command to perform
 *
 * Send down the scsi command described by @sic to the device below
 * the request queue @q.  If @file is non-NULL it's used to perform
 * fine-grained permission checks that allow users to send down
 * non-destructive SCSI commands.  If the caller has a struct gendisk
 * available it should be passed in as @disk to allow the low level
 * driver to use the information contained in it.  A non-NULL @disk
 * is only allowed if the caller knows that the low level driver doesn't
 * need it (e.g. in the scsi subsystem).
 *
 * Notes:
 *   -  This interface is deprecated - users should use the SG_IO
 *      interface instead, as this is a more flexible approach to
 *      performing SCSI commands on a device.
 *   -  The SCSI command length is determined by examining the 1st byte
 *      of the given command. There is no way to override this.
 *   -  Data transfers are limited to PAGE_SIZE
 *   -  The length (x + y) must be at least OMAX_SB_LEN bytes long to
 *      accommodate the sense buffer when an error occurs.
 *      The sense buffer is truncated to OMAX_SB_LEN (16) bytes so that
 *      old code will not be surprised.
 *   -  If a Unix error occurs (e.g. ENOMEM) then the user will receive
 *      a negative return and the Unix error code in 'errno'.
 *      If the SCSI command succeeds then 0 is returned.
 *      Positive numbers returned are the compacted SCSI error codes (4
 *      bytes in one int) where the lowest byte is the SCSI status.
 */
#define OMAX_SB_LEN 16          /* For backward compatibility */
int sg_scsi_ioctl(struct request_queue *q, struct gendisk *disk, fmode_t mode,
		struct scsi_ioctl_command __user *sic)
{
	struct request *rq;
	struct scsi_request *req;
	int err;
	unsigned int in_len, out_len, bytes, opcode, cmdlen;
	char *buffer = NULL;

	if (!sic)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

	/*
	 * get in an out lengths, verify they don't exceed a page worth of data
	 */
	if (get_user(in_len, &sic->inlen))
		return -EFAULT;
	if (get_user(out_len, &sic->outlen))
		return -EFAULT;
	if (in_len > PAGE_SIZE || out_len > PAGE_SIZE)
		return -EINVAL;
	if (get_user(opcode, sic->data))
		return -EFAULT;

	bytes = max(in_len, out_len);
	if (bytes) {
		buffer = kzalloc(bytes, q->bounce_gfp | GFP_USER| __GFP_NOWARN);
		if (!buffer)
			return -ENOMEM;

	}

	rq = blk_get_request(q, in_len ? REQ_OP_SCSI_OUT : REQ_OP_SCSI_IN,
			__GFP_RECLAIM);
	if (IS_ERR(rq)) {
		err = PTR_ERR(rq);
		goto error_free_buffer;
	}
	req = scsi_req(rq);

	cmdlen = COMMAND_SIZE(opcode);

	/*
	 * get command and data to send to device, if any
	 */
	err = -EFAULT;
	req->cmd_len = cmdlen;
	if (copy_from_user(req->cmd, sic->data, cmdlen))
		goto error;

	if (in_len && copy_from_user(buffer, sic->data + cmdlen, in_len))
		goto error;

	err = blk_verify_command(req->cmd, mode & FMODE_WRITE);
	if (err)
		goto error;

	/* default.  possible overriden later */
	req->retries = 5;

	switch (opcode) {
	case SEND_DIAGNOSTIC:
	case FORMAT_UNIT:
		rq->timeout = FORMAT_UNIT_TIMEOUT;
		req->retries = 1;
		break;
	case START_STOP:
		rq->timeout = START_STOP_TIMEOUT;
		break;
	case MOVE_MEDIUM:
		rq->timeout = MOVE_MEDIUM_TIMEOUT;
		break;
	case READ_ELEMENT_STATUS:
		rq->timeout = READ_ELEMENT_STATUS_TIMEOUT;
		break;
	case READ_DEFECT_DATA:
		rq->timeout = READ_DEFECT_DATA_TIMEOUT;
		req->retries = 1;
		break;
	default:
		rq->timeout = BLK_DEFAULT_SG_TIMEOUT;
		break;
	}

	if (bytes && blk_rq_map_kern(q, rq, buffer, bytes, __GFP_RECLAIM)) {
		err = DRIVER_ERROR << 24;
		goto error;
	}

	blk_execute_rq(q, disk, rq, 0);

	err = req->result & 0xff;	/* only 8 bit SCSI status */
	if (err) {
		if (req->sense_len && req->sense) {
			bytes = (OMAX_SB_LEN > req->sense_len) ?
				req->sense_len : OMAX_SB_LEN;
			if (copy_to_user(sic->data, req->sense, bytes))
				err = -EFAULT;
		}
	} else {
		if (copy_to_user(sic->data, buffer, out_len))
			err = -EFAULT;
	}
	
error:
	blk_put_request(rq);

error_free_buffer:
	kfree(buffer);

	return err;
}
EXPORT_SYMBOL_GPL(sg_scsi_ioctl);

/* Send basic block requests */
static int __blk_send_generic(struct request_queue *q, struct gendisk *bd_disk,
			      int cmd, int data)
{
	struct request *rq;
	int err;

	rq = blk_get_request(q, REQ_OP_SCSI_OUT, __GFP_RECLAIM);
	if (IS_ERR(rq))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return PTR_ERR(rq);
}
	rq->timeout = BLK_DEFAULT_SG_TIMEOUT;
	scsi_req(rq)->cmd[0] = cmd;
	scsi_req(rq)->cmd[4] = data;
	scsi_req(rq)->cmd_len = 6;
	blk_execute_rq(q, bd_disk, rq, 0);
	err = scsi_req(rq)->result ? -EIO : 0;
	blk_put_request(rq);

	return err;
}

static inline int blk_send_start_stop(struct request_queue *q,
				      struct gendisk *bd_disk, int data)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return __blk_send_generic(q, bd_disk, GPCMD_START_STOP_UNIT, data);
}

int scsi_cmd_ioctl(struct request_queue *q, struct gendisk *bd_disk, fmode_t mode,
		   unsigned int cmd, void __user *arg)
{
	int err;

	if (!q)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENXIO;
}

	switch (cmd) {
		/*
		 * new sgv3 interface
		 */
		case SG_GET_VERSION_NUM:
			err = sg_get_version(arg);
			break;
		case SCSI_IOCTL_GET_IDLUN:
			err = scsi_get_idlun(q, arg);
			break;
		case SCSI_IOCTL_GET_BUS_NUMBER:
			err = scsi_get_bus(q, arg);
			break;
		case SG_SET_TIMEOUT:
			err = sg_set_timeout(q, arg);
			break;
		case SG_GET_TIMEOUT:
			err = sg_get_timeout(q);
			break;
		case SG_GET_RESERVED_SIZE:
			err = sg_get_reserved_size(q, arg);
			break;
		case SG_SET_RESERVED_SIZE:
			err = sg_set_reserved_size(q, arg);
			break;
		case SG_EMULATED_HOST:
			err = sg_emulated_host(q, arg);
			break;
		case SG_IO: {
			struct sg_io_hdr hdr;

			err = -EFAULT;
			if (copy_from_user(&hdr, arg, sizeof(hdr)))
				break;
			err = sg_io(q, bd_disk, &hdr, mode);
			if (err == -EFAULT)
				break;

			if (copy_to_user(arg, &hdr, sizeof(hdr)))
				err = -EFAULT;
			break;
		}
		case CDROM_SEND_PACKET: {
			struct cdrom_generic_command cgc;
			struct sg_io_hdr hdr;

			err = -EFAULT;
			if (copy_from_user(&cgc, arg, sizeof(cgc)))
				break;
			cgc.timeout = clock_t_to_jiffies(cgc.timeout);
			memset(&hdr, 0, sizeof(hdr));
			hdr.interface_id = 'S';
			hdr.cmd_len = sizeof(cgc.cmd);
			hdr.dxfer_len = cgc.buflen;
			err = 0;
			switch (cgc.data_direction) {
				case CGC_DATA_UNKNOWN:
					hdr.dxfer_direction = SG_DXFER_UNKNOWN;
					break;
				case CGC_DATA_WRITE:
					hdr.dxfer_direction = SG_DXFER_TO_DEV;
					break;
				case CGC_DATA_READ:
					hdr.dxfer_direction = SG_DXFER_FROM_DEV;
					break;
				case CGC_DATA_NONE:
					hdr.dxfer_direction = SG_DXFER_NONE;
					break;
				default:
					err = -EINVAL;
			}
			if (err)
				break;

			hdr.dxferp = cgc.buffer;
			hdr.sbp = cgc.sense;
			if (hdr.sbp)
				hdr.mx_sb_len = sizeof(struct request_sense);
			hdr.timeout = jiffies_to_msecs(cgc.timeout);
			hdr.cmdp = ((struct cdrom_generic_command __user*) arg)->cmd;
			hdr.cmd_len = sizeof(cgc.cmd);

			err = sg_io(q, bd_disk, &hdr, mode);
			if (err == -EFAULT)
				break;

			if (hdr.status)
				err = -EIO;

			cgc.stat = err;
			cgc.buflen = hdr.resid;
			if (copy_to_user(arg, &cgc, sizeof(cgc)))
				err = -EFAULT;

			break;
		}

		/*
		 * old junk scsi send command ioctl
		 */
		case SCSI_IOCTL_SEND_COMMAND:
			printk(KERN_WARNING "program %s is using a deprecated SCSI ioctl, please convert it to SG_IO\n", current->comm);
			err = -EINVAL;
			if (!arg)
				break;

			err = sg_scsi_ioctl(q, bd_disk, mode, arg);
			break;
		case CDROMCLOSETRAY:
			err = blk_send_start_stop(q, bd_disk, 0x03);
			break;
		case CDROMEJECT:
			err = blk_send_start_stop(q, bd_disk, 0x02);
			break;
		default:
			err = -ENOTTY;
	}

	return err;
}
EXPORT_SYMBOL(scsi_cmd_ioctl);

int scsi_verify_blk_ioctl(struct block_device *bd, unsigned int cmd)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (bd && bd == bd->bd_contains)
		return 0;

	/* Actually none of these is particularly useful on a partition,
	 * but they are safe.
	 */
	switch (cmd) {
	case SCSI_IOCTL_GET_IDLUN:
	case SCSI_IOCTL_GET_BUS_NUMBER:
	case SCSI_IOCTL_GET_PCI:
	case SCSI_IOCTL_PROBE_HOST:
	case SG_GET_VERSION_NUM:
	case SG_SET_TIMEOUT:
	case SG_GET_TIMEOUT:
	case SG_GET_RESERVED_SIZE:
	case SG_SET_RESERVED_SIZE:
	case SG_EMULATED_HOST:
		return 0;
	case CDROM_GET_CAPABILITY:
		/* Keep this until we remove the printk below.  udev sends it
		 * and we do not want to spam dmesg about it.   CD-ROMs do
		 * not have partitions, so we get here only for disks.
		 */
		return -ENOIOCTLCMD;
	default:
		break;
	}

	if (capable(CAP_SYS_RAWIO))
		return 0;

	/* In particular, rule out all resets and host-specific ioctls.  */
	printk_ratelimited(KERN_WARNING
			   "%s: sending ioctl %x to a partition!\n", current->comm, cmd);

	return -ENOIOCTLCMD;
}
EXPORT_SYMBOL(scsi_verify_blk_ioctl);

int scsi_cmd_blk_ioctl(struct block_device *bd, fmode_t mode,
		       unsigned int cmd, void __user *arg)
{
	int ret;

	ret = scsi_verify_blk_ioctl(bd, cmd);
	if (ret < 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ret;
}

	return scsi_cmd_ioctl(bd->bd_disk->queue, bd->bd_disk, mode, cmd, arg);
}
EXPORT_SYMBOL(scsi_cmd_blk_ioctl);

/**
 * scsi_req_init - initialize certain fields of a scsi_request structure
 * @req: Pointer to a scsi_request structure.
 * Initializes .__cmd[], .cmd, .cmd_len and .sense_len but no other members
 * of struct scsi_request.
 */
void scsi_req_init(struct scsi_request *req)
{
	memset(req->__cmd, 0, sizeof(req->__cmd));
	req->cmd = req->__cmd;
	req->cmd_len = BLK_MAX_CDB;
	req->sense_len = 0;
}
EXPORT_SYMBOL(scsi_req_init);

static int __init blk_scsi_ioctl_init(void)
{
	blk_set_cmd_filter_defaults(&blk_default_cmd_filter);
	return 0;
}
fs_initcall(blk_scsi_ioctl_init);
