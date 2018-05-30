import React, { Component, Fragment } from 'react';
import { connect } from 'react-redux';
import Paper from '@material-ui/core/Paper';
import Button from '@material-ui/core/Button';
import Snackbar from '@material-ui/core/Snackbar';
import IconButton from '@material-ui/core/IconButton';
import PlayArrow from '@material-ui/icons/PlayArrow';
import Stop from '@material-ui/icons/Stop';
import AutoRenew from '@material-ui/icons/AutoRenew';
import CloseIcon from '@material-ui/icons/Close';

import { play, stop, scan } from '../../services/controls';

import './style.css';

class Controls extends Component {
	state = {
		playing: false,
		snackOpen: false,
		snackMessage: '',
		scanDisabled: false,
	};

	togglePlay = async () => {
		let result;
		if (this.state.playing) result = await stop();
		else result = await play();
		console.log(result);
		if (result.success) this.setState({ playing: !this.state.playing });
		else
			this.setState({
				snackOpen: true,
				snackMessage: `An error occured! ${JSON.stringify(result.error)}`,
			});
	};

	scanPlayers = async () => {
		this.setState({ scanDisabled: true }, async () => {
			const result = await scan();
			if (result.error && result.error.filter(e => e !== null).length) {
				this.setState({
					snackOpen: true,
					snackMessage: `The following IPs are not reachable: ${result.error
						.filter(e => e !== null)
						.join(', ')}`,
				});
			}
			this.setState({ scanDisabled: false });
		});
	};

	handleSnackClose = () => {
		this.setState({ snackOpen: false, snackMessage: '' });
	};

	render() {
		const disabled = !this.state.playing && !this.props.scanned;
		return (
			<Paper className="controls" elevation={4}>
				{this.state.playing ? (
					<Button variant="raised" color="secondary" onClick={this.togglePlay}>
						<Stop />
						Stop
					</Button>
				) : (
					<Button
						variant="raised"
						className="controls__play"
						disabled={disabled}
						onClick={this.togglePlay}
					>
						<PlayArrow />
						Play
					</Button>
				)}
				<Button
					variant="raised"
					color="primary"
					onClick={this.scanPlayers}
					disabled={this.state.scanDisabled}
					className={this.state.scanDisabled ? 'rotating' : ''}
				>
					<AutoRenew />
					Scan
				</Button>
				<Snackbar
					anchorOrigin={{
						vertical: 'bottom',
						horizontal: 'left',
					}}
					open={this.state.snackOpen}
					autoHideDuration={6000}
					onClose={this.handleSnackClose}
					message={<span>{this.state.snackMessage}</span>}
					action={[
						<IconButton
							key="close"
							aria-label="Close"
							color="inherit"
							onClick={this.handleSnackClose}
						>
							<CloseIcon />
						</IconButton>,
					]}
				/>
			</Paper>
		);
	}
}

export default connect(state => state)(Controls);
