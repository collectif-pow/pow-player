const path = require('path');
const webpack = require('webpack');
const HtmlWebpackPlugin = require('html-webpack-plugin');

const plugins = [new HtmlWebpackPlugin({ template: './admin/src/index.html' })];

console.log(__dirname);

module.exports = {
	entry: ['babel-polyfill', './admin/src/main.js'],
	output: {
		path: __dirname + '/admin/public',
	},
	module: {
		rules: [
			// js
			{
				test: /\.js$/,
				use: ['babel-loader'],
				include: [path.resolve(__dirname, 'admin', 'src')],
			},
			// css
			{ test: /\.css$/, use: ['style-loader', 'css-loader'] },
		],
	},
	plugins,
	devServer: {
		host: '0.0.0.0',
		historyApiFallback: true,
	},
};
