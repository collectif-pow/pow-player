import C from '../constants';
import { store } from '../store';
import * as S from '../actions/scan';
import * as SD from '../actions/scanned';

export const play = async () => {
	const res = await fetch('//localhost:3000/api/play');
	const json = await res.json();
	return json;
};

export const stop = async () => {
	const res = await fetch('//localhost:3000/api/stop');
	const json = await res.json();
	return json;
};

export const scan = async () => {
	const res = await fetch('//localhost:3000/api/scan');
	const json = await res.json();
	const missingIps = json.error ? json.error : [];
	store.dispatch(S.scan(missingIps));
	store.dispatch(SD.scanned(true));
	return json;
};
